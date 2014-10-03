#include "taskwindow.h"
#include "application.hpp"
#include "taskview.hpp"
#include "taskmanager.hpp"
#include "rest-client/baserestclient.hpp"
#include "rest-client/milestone.hpp"
#include "rest-client/issue.hpp"

#include <QListWidget>
#include <QListWidgetItem>
#include <QComboBox>
#include <QLabel>
#include <QListWidgetItem>
#include <QGridLayout>
#include <QPushButton>

TaskWindow::TaskWindow(QWidget *parent) :
    QWidget(parent)
{
    QGridLayout *mainLayout = new QGridLayout;

    mainLayout->addWidget(new QLabel(tr("Project: "), this), 0, 0);
    mProjectNameLabel = new QLabel(this);
    mainLayout->addWidget(mProjectNameLabel, 0, 1);

    mainLayout->addWidget(new QLabel(tr("Milestones: "), this), 1, 0);
    mMilestoneComboBox = new QComboBox(this);
    connect(mMilestoneComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(selectedMilestoneChanged(int)));
    mainLayout->addWidget(mMilestoneComboBox, 1, 1, 1, 2);

    mTaskList = new QListWidget(this);
    connect(mTaskList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(openViewWindow(QListWidgetItem*)));
    mainLayout->addWidget(mTaskList, 2, 0, 1, 3);

    mViewButton = new QPushButton(tr("View task"), this);
    connect(mViewButton, SIGNAL(clicked()), this, SLOT(openViewWindow()));
    mainLayout->addWidget(mViewButton, 3, 1);

    setLayout(mainLayout);
    setWindowTitle(tr("Tasks"));

    mTaskView = new TaskView(this);
}

TaskWindow::~TaskWindow()
{
    delete mProjectNameLabel;
    delete mMilestoneComboBox;
    delete mTaskList;
    delete mViewButton;
    delete mTaskView;
}

void TaskWindow::openViewWindow()
{
    QListWidgetItem *item = mTaskList->selectedItems().at(0);
    openViewWindow(item);
}

void TaskWindow::openViewWindow(QListWidgetItem *item)
{
    if (!item)
        return;
    Issue *issue = mIssueMapping.value(item);
    mTaskView->setIssue(issue);
    mTaskView->exec();
}

void TaskWindow::milestonesChanged(QList<Milestone *> &list, bool success)
{
    if (success)
    {
        mMilestoneComboBox->clear();
        for (int i = 0; i < list.size(); ++i)
        {
            Milestone *mile = list.at(i);
            mMilestoneComboBox->addItem(mile->getVersion());
        }
    }
}

void TaskWindow::selectedMilestoneChanged(int index)
{
    QString version = mMilestoneComboBox->itemText(index);
    QString projectName = qApp->settings("Tracker_ProjectShort").toString();
    QMap<BaseRestClient::IssueFilter, QVariant> params;
    params[BaseRestClient::MilestoneFilter] = version;
    BaseRestClient *client = TaskManager::getInstance()->getClientByName("YouTrack");
    client->getIssues(projectName, params);
}

void TaskWindow::issuesChanged(QList<Issue *> &list, bool success)
{
    if (success)
    {
        QList<Issue*> oldIssues = mIssueMapping.values();
        for (int i = 0; i < oldIssues.size(); ++i)
            delete oldIssues.at(i);
        mTaskList->clear();
        mIssueMapping.clear();
        QIcon complete(":/media/icons/state_complete.png");
        QIcon working(":/media/icons/state_working.png");
        QIcon delayed(":/media/icons/state_delayed.png");
        for (int i = 0; i < list.size(); ++i)
        {
            Issue *issue = list.at(i);
            QIcon icon;
            if (issue->getState() == "Open")
                icon = working;
            else if (issue->getState() == "Fixed")
                icon = complete;
            else
                icon = delayed;
            QListWidgetItem *item = new QListWidgetItem(icon, issue->getSummry(), mTaskList);
            mIssueMapping[item] = issue;
        }
    }
}

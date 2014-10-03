#include "trackersettingswidget.hpp"
#include "application.hpp"
#include "taskmanager.hpp"
#include "rest-client/baserestclient.hpp"
#include "rest-client/project.hpp"
#include <QLineEdit>
#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

#include <QDomDocument>

TrackerSettingsWidget::TrackerSettingsWidget(QWidget *parent) :
    AbstractSettingsWidget(parent)
{
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(new QLabel(tr("YouTrack Login:"), this), 0, 0);
    mLoginEdit = new QLineEdit(this);
    mainLayout->addWidget(mLoginEdit, 0, 1, 1, 2);
    mainLayout->addWidget(new QLabel(tr("YouTrack password:"), this), 1, 0);
    mPasswordEdit = new QLineEdit(this);
    mPasswordEdit->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    mainLayout->addWidget(mPasswordEdit, 1, 1, 1, 2);

    mUrlEdit = new QLineEdit(this);
    mainLayout->addWidget(new QLabel(tr("Url: "), this), 2, 0);
    mainLayout->addWidget(mUrlEdit, 2, 1, 1, 2);

    mSuccessLabel = new QLabel(tr("Success"), this);
    mSuccessLabel->hide();
    mFailLabel = new QLabel(tr("Fail"), this);
    mFailLabel->hide();
    mainLayout->addWidget(mSuccessLabel, 3, 1);
    mainLayout->addWidget(mFailLabel, 3, 1);

    mTestButton = new QPushButton(tr("Test connection"), this);
    connect(mTestButton, SIGNAL(clicked()), this, SLOT(testConnection()));
    mainLayout->addWidget(mTestButton, 3, 2);

    mainLayout->addWidget(new QLabel(tr("Projects: "), this), 4, 0);
    mProjectList = new QComboBox(this);
    connect(mProjectList, SIGNAL(currentIndexChanged(int)), this, SLOT(projectChanged(int)));
    mainLayout->addWidget(mProjectList, 4, 1, 1, 2);

    setLayout(mainLayout);
}

TrackerSettingsWidget::~TrackerSettingsWidget()
{
    delete mLoginEdit;
    delete mPasswordEdit;
    delete mProjectList;
    delete mSuccessLabel;
    delete mFailLabel;
    delete mProjectList;
    delete mTestButton;
}

QDomNode *TrackerSettingsWidget::serialize()
{
    QDomDocument* doc = new QDomDocument;
    QDomElement root = doc->createElement(settingsGroup());
    doc->appendChild(root);

    QDomElement loginRoot = doc->createElement("login");
    loginRoot.setAttribute("value", mLoginEdit->text());

    QDomElement passRoot = doc->createElement("password");
    passRoot.setAttribute("value", mPasswordEdit->text());

    QDomElement urlRoot = doc->createElement("endpoint");
    urlRoot.setAttribute("value", mUrlEdit->text());

    root.appendChild(loginRoot);
    root.appendChild(passRoot);
    root.appendChild(urlRoot);

    QDomElement projNode = doc->createElement("projects");
    for (int i = 0; i < mProjectList->count(); ++i)
    {
        QDomElement proj = doc->createElement("project");
        proj.setAttribute("name", mProjectList->itemText(i));
        proj.setAttribute("shortName", mProjectList->itemData(i).toString());
        projNode.appendChild(proj);
    }

    root.appendChild(projNode);

    return doc;
}

void TrackerSettingsWidget::deserialize(const QDomNode *node)
{
    mProjectList->clear();
    QDomNode itemNode = node->namedItem("login");
    if (!itemNode.isNull())
    {
        QDomNode attr = itemNode.attributes().namedItem("value");
        if (!attr.isNull())
        {
            mLoginEdit->setText(attr.nodeValue());
        }
    }
    itemNode = node->namedItem("password");
    if (!itemNode.isNull())
    {
        QDomNode attr = itemNode.attributes().namedItem("value");
        if (!attr.isNull())
        {
            mPasswordEdit->setText(attr.nodeValue());
        }
    }
    itemNode = node->namedItem("endpoint");
    if (!itemNode.isNull())
    {
        QDomNode attr = itemNode.attributes().namedItem("value");
        if (!attr.isNull())
        {
            mUrlEdit->setText(attr.nodeValue());
        }
    }
    QDomNode projectsRootNode = node->namedItem("projects");
    if (!projectsRootNode.isNull())
    {
        QDomNodeList projects = projectsRootNode.childNodes();
        for (int i = 0; i < projects.size(); ++i)
        {
            QDomNode proj = projects.item(i);
            QDomNode nameAttr = proj.attributes().namedItem("name");
            QString name, shortName;
            if (!nameAttr.isNull())
            {
                name = nameAttr.nodeValue();
            }
            nameAttr = proj.attributes().namedItem("shortName");
            if (!nameAttr.isNull())
                shortName = nameAttr.nodeValue();
            if (!name.isEmpty() && !shortName.isEmpty())
                mProjectList->addItem(name, shortName);
        }
    }
    testConnection();
}

void TrackerSettingsWidget::saveSettings()
{
    const QString prefix = settingsGroup() + "_";
    qApp->addSettings(prefix + "Login", mLoginEdit->text());
    qApp->addSettings(prefix + "Password", mPasswordEdit->text());
    qApp->addSettings(prefix + "Url", mUrlEdit->text());
    qApp->addSettings(prefix + "Project", mProjectList->currentText());
    qApp->addSettings(prefix + "ProjectShort", mProjectList->itemData(mProjectList->currentIndex()).toString());
}

void TrackerSettingsWidget::testConnection()
{
    BaseRestClient *client = TaskManager::getInstance()->getClientByName("YouTrack");
    if (client)
    {
        mProjectList->clear();
        client->setEndPointUrl(mUrlEdit->text());
        connect(client, SIGNAL(loginComplete(bool)), this, SLOT(loginResult(bool)));
        client->login(mLoginEdit->text(), mPasswordEdit->text());
    }
    else
    {
        mFailLabel->show();
    }
}

void TrackerSettingsWidget::loginResult(bool success)
{
    mSuccessLabel->hide();
    mFailLabel->hide();
    success ? mSuccessLabel->show() : mFailLabel->show();
    if (success)
    {
        BaseRestClient *client = TaskManager::getInstance()->getClientByName("YouTrack");
        connect(client, SIGNAL(getAllProjectsComplete(QList<Project*>&,bool)), this, SLOT(projectListComplete(QList<Project*>&,bool)));
        client->getAllProjects();
    }
}

void TrackerSettingsWidget::projectListComplete(QList<Project *> &list, bool success)
{
    if (success)
    {
        for (int i = 0; i < list.size(); ++i)
        {
            Project *proj = list.at(i);
            mProjectList->addItem(proj->getProjectName(), proj->getShortProjectName());
            delete proj;
        }
    }
    saveSettings();
}

void TrackerSettingsWidget::projectChanged(int index)
{
    QString shortName = mProjectList->itemData(index).toString();
    BaseRestClient *client = TaskManager::getInstance()->getClientByName("YouTrack");
    client->getProjectMilestones(shortName);
}

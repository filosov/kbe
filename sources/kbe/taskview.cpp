#include "taskview.hpp"
#include "rest-client/issue.hpp"
#include <QTextEdit>
#include <QVBoxLayout>

const QString HTMLTemplate = "<!DOCTYPE html>"
        "<html>"
        "<head>"
        "<meta charset=\"utf-8\" />"
        "<title></title>"
        "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
        "</head>"
        "<body>"
        "<table border=\"1px\" height=\"100px\" width=\"500px\" cellpadding=\"5\" cellspacing=\"5\" style=\"border:solid 3px #68dc00; padding:5px;\">"
        "<tr>"
        "<td style=\"border-bottom:1px; border-top:1px; border-left:1px; border-right:1px\"> Project: </td>"
        "<td style=\"border-bottom:1px; border-top:1px; border-left:1px; border-right:1px\"> %1 </td>"
        "<td style=\"border-bottom:1px; border-top:1px; border-left:1px; border-right:1px\"> Priority: </td>"
        "<td style=\"border-bottom:1px; border-top:1px; border-left:1px; border-right:1px\"> %4 </td>"
        "</tr>"
        "<tr>"
        "<td style=\"border-bottom:1px; border-top:1px; border-left:1px; border-right:1px\"> Milestone: </td>"
        "<td style=\"border-bottom:1px; border-top:1px; border-left:1px; border-right:1px\"> %2 </td>"
        "<td style=\"border-bottom:1px; border-top:1px; border-left:1px; border-right:1px\"> Type: </td>"
        "<td style=\"border-bottom:1px; border-top:1px; border-left:1px; border-right:1px\"> %5 </td>"
        "</tr>"
        "<tr>"
        "<td style=\"border-bottom:1px; border-top:1px; border-left:1px; border-right:1px\"> State: </td>"
        "<td style=\"border-bottom:1px; border-top:1px; border-left:1px; border-right:1px\"> %3 </td>"
        "<td style=\"border-bottom:1px; border-top:1px; border-left:1px; border-right:1px\">  </td>"
        "<td style=\"border-bottom:1px; border-top:1px; border-left:1px; border-right:1px\">  </td>"
        "</tr>"
        "<tr>"
        "<td colspan=\"4\" rules=\"cols\" style=\"border-bottom:1px; border-left:1px; border-right:1px;\"> Description:</td>"
        "</tr>"
        "<tr>"
        "<td colspan=\"4\" rules=\"cols\" style=\"border-top:1px; border-left:1px; border-right:1px;border-bottom:1px;\"> %6 </td>"
        "</tr>"
        "</table>"
        "</body>"
        "</html>";

TaskView::TaskView(QWidget *parent) :
    QDialog(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mView = new QTextEdit(this);
    mView->setReadOnly(true);
    mainLayout->addWidget(mView);

    setLayout(mainLayout);
    setWindowTitle(tr("Task view"));
    setMinimumSize(800, 300);
}

TaskView::~TaskView()
{
    delete mView;
}

void TaskView::setIssue(const Issue *const issue)
{
    QString html = HTMLTemplate
            .arg(issue->getProjectName())
            .arg(issue->getMilestone())
            .arg(issue->getState())
            .arg(issue->getPriority())
            .arg(issue->getType())
            .arg(issue->getDescription());
    mView->setHtml(html);
}

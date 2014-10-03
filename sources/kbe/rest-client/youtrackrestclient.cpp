#include "youtrackrestclient.hpp"
#include "project.hpp"
#include "milestone.hpp"
#include "issue.hpp"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkCookie>

#include <QStringList>
#include <QDomDocument>

#include <QRegExp>

#include <QDebug>

const QString LoginEndPoint = "/rest/user/login";
const QString ProjectEndPoint = "/rest/project/all?verbose=true";
const QString IssuesEndPoint = "/rest/issue/byproject/%1?with=projectShortName"
        "&with=summary"
        "&with=description"
        "&with=priority"
        "&with=type"
        "&with=state"
        "&with=fix versions";

YouTrackRestClient::YouTrackRestClient(QObject *parent) :
    BaseRestClient(parent)
{
    connect(mNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
}

YouTrackRestClient::~YouTrackRestClient()
{
}

void YouTrackRestClient::login(const QString &login, const QString &password)
{
    QNetworkRequest request;
    request.setUrl(mEndpointUrl + LoginEndPoint);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QString str = "login=%1&password=%2";
    str = str.arg(login).arg(password);
    QNetworkReply *reply = mNetworkManager->post(request, QByteArray(str.toAscii()));
    addReply(reply, Login);
}

void YouTrackRestClient::logout()
{
}

void YouTrackRestClient::getAllProjects()
{
    QNetworkRequest request;
    request.setUrl(mEndpointUrl + ProjectEndPoint);
    QNetworkReply *reply = mNetworkManager->get(request);
    addReply(reply, GetProjects);
}

void YouTrackRestClient::getProjectMilestones(const Project *const project)
{
    getProjectMilestones(project->getShortProjectName());
}

void YouTrackRestClient::getProjectMilestones(const QString &project)
{
    QNetworkRequest request;
    request.setAttribute(QNetworkRequest::User, project);
    request.setUrl(mEndpointUrl + ProjectEndPoint);
    QNetworkReply *reply = mNetworkManager->get(request);
    addReply(reply, GetMilestones);
}

void YouTrackRestClient::replyFinished(QNetworkReply *reply)
{
    OperationType type = mReplyQueue.value(reply, Unknown);
    qDebug() << "Reply received. Operation type :" << type;
    const QString content = reply->readAll();
    switch (type)
    {
    case Login :
    {
        handleLoginResponce(content);
        break;
    }
    case Logout :
    {
        break;
    }
    case GetProjects :
    {
        handleGetProjectsResponce(content);
        break;
    }
    case GetMilestones :
    {
        const QString name = reply->request().attribute(QNetworkRequest::User).toString();
        handleGetMilestonesResponce(content, name);
        break;
    }
    case GetIssues :
    {
        handleGetIssuesResponce(content);
        break;
    }
    default :
        break;
    }
}

void YouTrackRestClient::handleLoginResponce(const QString &content)
{
    if (receivers(SIGNAL(loginComplete(bool))) == 0)
        return;
    QDomDocument doc;
    QString errorMsg = "";
    int errRow = -1, errCol = -1;
    doc.setContent(content, &errorMsg, &errRow, &errCol);
    if (!errorMsg.isEmpty())
    {
        qDebug() << "Responce parsing is failed with message";
        qDebug() << errorMsg;
        qDebug() << "Place: " << errRow << "," << errCol;
    }
    bool res = !doc.elementsByTagName("login").isEmpty();
    emit loginComplete(res);
}

void YouTrackRestClient::handleGetProjectsResponce(const QString &content)
{
    if (receivers(SIGNAL(getAllProjectsComplete(QList<Project*>&,bool))) == 0)
        return;
    const QString cont = QString::fromUtf8(content.toAscii());
    QDomDocument doc;
    QString errorMsg = "";
    int errRow = -1, errCol = -1;
    doc.setContent(cont, &errorMsg, &errRow, &errCol);
    if (!errorMsg.isEmpty())
    {
        qDebug() << "Responce parsing is failed with message";
        qDebug() << errorMsg;
        qDebug() << "Place: " << errRow << "," << errCol;
    }

    QDomNodeList list = doc.elementsByTagName("error");
    QList<Project*> descriptionList;
    if (!list.isEmpty())
    {
        qDebug() << "Error received: " << list.at(0).nodeValue();
        emit getAllProjectsComplete(descriptionList, false);
        return;
    }

    QDomNode projectsNode = doc.elementsByTagName("projects").at(0);
    QDomNodeList projectList = projectsNode.childNodes();
    for (int i = 0; i < projectList.count(); ++i)
    {
        QDomNode node = projectList.at(i);
        QDomNamedNodeMap attrs = node.attributes();
        QString name = attrs.namedItem("name").nodeValue();
        QString shortName = attrs.namedItem("shortName").nodeValue();
        QString descr = attrs.namedItem("description").nodeValue();
        descriptionList.append(new Project(name, shortName, descr));
    }
    emit getAllProjectsComplete(descriptionList, true);
}

void YouTrackRestClient::handleGetMilestonesResponce(const QString &content, const QString &projectShortName)
{
    if (receivers(SIGNAL(getProjectMilestonesComplete(QList<Milestone*>&,bool))) == 0)
        return;
    const QString cont = QString::fromUtf8(content.toAscii());
    QDomDocument doc;
    QString errorMsg = "";
    int errRow = -1, errCol = -1;
    doc.setContent(cont, &errorMsg, &errRow, &errCol);
    if (!errorMsg.isEmpty())
    {
        qDebug() << "Responce parsing is failed with message";
        qDebug() << errorMsg;
        qDebug() << "Place: " << errRow << "," << errCol;
    }

    QList<Milestone*> descriptionList;
    QDomNodeList list = doc.elementsByTagName("error");
    if (!list.isEmpty())
    {
        qDebug() << "Error received: " << list.at(0).nodeValue();
        emit getProjectMilestonesComplete(descriptionList, false);
        return;
    }

    QDomNode projectsNode = doc.elementsByTagName("projects").at(0);
    QDomNodeList projectList = projectsNode.childNodes();
    for (int i = 0; i < projectList.count(); ++i)
    {
        QDomNode node = projectList.at(i);
        QDomNamedNodeMap attrs = node.attributes();
        QString name = attrs.namedItem("shortName").nodeValue();
        if (name == projectShortName)
        {
            QString versions = attrs.namedItem("versions").nodeValue();
            QRegExp exp("([^,\\]\\[]+)");
            int offset = 0;
            while (exp.indexIn(versions, offset) != -1)
            {
                QString c = exp.cap(0);
                offset += c.size() + 1;
                QString project = attrs.namedItem("name").nodeValue();
                Milestone *mile = new Milestone(project, c.trimmed(), "", "");
                descriptionList.append(mile);
            }
        }
    }
    emit getProjectMilestonesComplete(descriptionList, true);
}

void YouTrackRestClient::handleGetIssuesResponce(const QString &content)
{
    if (receivers(SIGNAL(getIssuesComplete(QList<Issue*>&,bool))) == 0)
        return;
    const QString cont = QString::fromUtf8(content.toAscii());
    QDomDocument doc;
    QString errorMsg = "";
    int errRow = -1, errCol = -1;
    doc.setContent(cont, &errorMsg, &errRow, &errCol);
    if (!errorMsg.isEmpty())
    {
        qDebug() << "Responce parsing is failed with message";
        qDebug() << errorMsg;
        qDebug() << "Place: " << errRow << "," << errCol;
    }

    QList<Issue*> descriptionList;

    QDomNodeList list = doc.elementsByTagName("error");
    if (!list.isEmpty())
    {
        qDebug() << "Error received: " << list.at(0).nodeValue();
        emit getIssuesComplete(descriptionList, false);
        return;
    }
    QDomNode resultsNode = doc.elementsByTagName("issues").at(0);
    QDomNodeList result = resultsNode.childNodes();
    for (int i = 0; i < result.count(); ++i)
    {
        QString projShortName, summary, description, priority,
                state, type, milestone;

        QDomNode node = result.at(i);
        QDomNodeList issueFiledsList = node.childNodes();
        for (int j = 0; j < issueFiledsList.count(); ++j)
        {
            QDomNode field = issueFiledsList.at(j);
            QString value = field.namedItem("value").firstChild().nodeValue();
            QString fieldName = field.attributes().namedItem("name").nodeValue();
            if (fieldName == "projectShortName")
                projShortName = value;
            else if (fieldName == "summary")
                summary = value;
            else if (fieldName == "description")
                description = value;
            else if (fieldName == "Priority")
                priority = value;
            else if (fieldName == "State")
                state = value;
            else if (fieldName == "Type")
                type = value;
            else if (fieldName == "Fix versions")
                milestone = value;
        }
        descriptionList.append(new Issue(summary, projShortName, milestone, state, priority, type, description));
    }
    emit getIssuesComplete(descriptionList, true);
}

void YouTrackRestClient::getIssues(const QString &project, const QMap<IssueFilter, QVariant> &filters)
{
    QString params;
    foreach (const IssueFilter filter, filters.keys())
    {
        QString fil = "%1:%2";
        switch (filter)
        {
        case StateFilter :
        {
            fil = fil.arg("state");
            break;
        }
        case PriorityFilter :
        {
            fil = fil.arg("priority");
            break;
        }
        case MilestoneFilter :
        {
            fil = fil.arg("fix versions");
            break;
        }
        case AssigneeFilter :
        {
            fil = fil.arg("assignee");
            break;
        }
        }
        fil = fil.arg(filters.value(filter).toString());

        params += QString("&filter=%1").arg(fil);
    }

    QNetworkRequest request;
    QString url = IssuesEndPoint.arg(project) + params;
    request.setUrl(mEndpointUrl + url);
    QNetworkReply *reply = mNetworkManager->get(request);
    addReply(reply, GetIssues);
}

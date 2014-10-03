#ifndef __BASERESTCLIENT_HPP__
#define __BASERESTCLIENT_HPP__

#include <QObject>
#include <QMap>

class Project;
class Milestone;
class Issue;
class QNetworkAccessManager;
class QNetworkReply;

class BaseRestClient : public QObject
{
    Q_OBJECT
protected:
    enum OperationType
    {
        Login,
        Logout,
        GetProjects,
        GetMilestones,
        GetIssues,

        Unknown
    };
public:
    enum IssueFilter
    {
        StateFilter,
        PriorityFilter,
        MilestoneFilter,
        AssigneeFilter
    };

    explicit BaseRestClient(QObject *parent = 0);
    virtual ~BaseRestClient();

    virtual bool addReply(QNetworkReply* reply, OperationType type);

    void setEndPointUrl(const QString& url) { mEndpointUrl = url; }
    const QString& getEndPointUrl() const { return mEndpointUrl; }

    virtual void login(const QString& login, const QString& password) = 0;
    virtual void logout() = 0;
    virtual void getAllProjects() = 0;
    virtual void getProjectMilestones(const Project* const project) = 0;
    virtual void getProjectMilestones(const QString& project) = 0;
    virtual void getIssues(const QString& project, const QMap<IssueFilter, QVariant>& filters) = 0;

    virtual QString clientName() const = 0;
signals:
    void loginComplete(bool success);
    void logoutComplete(bool success);
    void getAllProjectsComplete(QList<Project*>& list, bool success);
    void getProjectMilestonesComplete(QList<Milestone*>& list, bool success);
    void getIssuesComplete(QList<Issue*>& list, bool success);
public slots:

protected:
    QMap<QNetworkReply*, OperationType> mReplyQueue;
    QNetworkAccessManager *mNetworkManager;
    QString mEndpointUrl;
};

#endif // __BASERESTCLIENT_HPP__

#ifndef __YOUTRACKRESTCLIENT_HPP__
#define __YOUTRACKRESTCLIENT_HPP__

#include "baserestclient.hpp"

class QNetworkCookie;
class QNetworkReply;

class YouTrackRestClient : public BaseRestClient
{
    Q_OBJECT
public:
    explicit YouTrackRestClient(QObject *parent = 0);
    ~YouTrackRestClient();
    
    void login(const QString& login, const QString& password);
    void logout();
    void getAllProjects();
    void getProjectMilestones(const Project* const project);
    void getProjectMilestones(const QString& project);
    void getIssues(const QString& project, const QMap<IssueFilter, QVariant>& filters);

    QString clientName() const { return "YouTrack"; }
signals:
    
public slots:

private slots:
    void replyFinished(QNetworkReply* reply);

private:
    void handleLoginResponce(const QString& content);
    void handleGetProjectsResponce(const QString& content);
    void handleGetMilestonesResponce(const QString& content, const QString& projectShortName);
    void handleGetIssuesResponce(const QString& content);
    QNetworkCookie *mAccessCookie;
};

#endif // YOUTRACKRESTCLIENT_HPP

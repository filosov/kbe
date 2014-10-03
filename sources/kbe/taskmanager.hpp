#ifndef TASKMANAGER_HPP
#define TASKMANAGER_HPP

#include <QObject>
#include <QMap>

class BaseRestClient;
class TaskWindow;

class TaskManager : public QObject
{
    Q_OBJECT
public:
    static TaskManager* getInstance();
    static void destroyInstance();

    bool registerRESTClient(BaseRestClient* client);
    bool unregisterRESTClient(const QString& clientName);
    bool unregisterRESTClient(BaseRestClient* client);
    void unregisterAllClients();

    TaskWindow* taskWindow() const { return mTaskWindow; }

    BaseRestClient *getClientByName(const QString& clientName);
private:
    explicit TaskManager(QObject *parent = 0);
    ~TaskManager();
    static TaskManager* mInstance;
private:
    QMap<QString, BaseRestClient*> mRegisteredClients;
    TaskWindow *mTaskWindow;
};

#endif // TASKMANAGER_HPP

#include "taskmanager.hpp"
#include "rest-client/baserestclient.hpp"
#include "taskwindow.h"

TaskManager* TaskManager::mInstance = 0;

TaskManager::TaskManager(QObject *parent) :
    QObject(parent)
{
    mTaskWindow = new TaskWindow;
}

TaskManager::~TaskManager()
{
    delete mTaskWindow;
    unregisterAllClients();
}

TaskManager *TaskManager::getInstance()
{
    if (!mInstance)
        mInstance = new TaskManager;
    return mInstance;
}

void TaskManager::destroyInstance()
{
    if (mInstance)
    {
        delete mInstance;
        mInstance = 0;
    }
}

bool TaskManager::registerRESTClient(BaseRestClient *client)
{
    const QString name = client->clientName();
    if (mRegisteredClients.contains(name))
        return false;
    mRegisteredClients[name] = client;
    connect(client, SIGNAL(getProjectMilestonesComplete(QList<Milestone*>&,bool)),
            mTaskWindow, SLOT(milestonesChanged(QList<Milestone*>&,bool)));
    connect(client, SIGNAL(getIssuesComplete(QList<Issue*>&,bool)),
            mTaskWindow, SLOT(issuesChanged(QList<Issue*>&,bool)));

    return true;
}

bool TaskManager::unregisterRESTClient(const QString &clientName)
{
    if (!mRegisteredClients.contains(clientName))
        return false;
    delete mRegisteredClients.value(clientName);
    mRegisteredClients.remove(clientName);
    return true;
}

bool TaskManager::unregisterRESTClient(BaseRestClient *client)
{
    const QString name = client->clientName();
    return unregisterRESTClient(name);
}

void TaskManager::unregisterAllClients()
{
    foreach (QString entry, mRegisteredClients.keys())
    {
        delete mRegisteredClients.value(entry);
    }
    mRegisteredClients.clear();
}

BaseRestClient *TaskManager::getClientByName(const QString &clientName)
{
    return mRegisteredClients.value(clientName);
}

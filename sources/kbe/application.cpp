#include "application.hpp"
#include "taskmanager.hpp"
#include "rest-client/youtrackrestclient.hpp"

Application::Application(int argc, char *argv[])
    :QApplication(argc, argv)
{
    TaskManager::getInstance()->registerRESTClient(new YouTrackRestClient);
}

Application::~Application()
{
    TaskManager::destroyInstance();
}

bool Application::addSettings(const QString &key, const QVariant value, bool overrideExistance)
{
    if (!overrideExistance && mApplicationSettings.contains(key))
        return false;
    mApplicationSettings[key] = value;
    return true;
}

const QVariant Application::settings(const QString &key)
{
    return mApplicationSettings.value(key);
}

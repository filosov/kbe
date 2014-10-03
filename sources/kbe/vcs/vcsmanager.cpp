#include "VCSManager.hpp"
#include "vcsinterface.hpp"
#include <QDir>

const QString VCSManager::GIT = "git";
const QString VCSManager::SVN = "svn";
const QString VCSManager::MERCURIAL = "hg";

VCSManager* VCSManager::mInstance = 0;

VCSManager::VCSManager()
    : mWorkingRepoType("")
{
    mVCSTypes.append(VCSManager::GIT);
    mVCSTypes.append(VCSManager::SVN);
    mVCSTypes.append(VCSManager::MERCURIAL);
}

VCSManager::~VCSManager()
{
    unregisterAllVCSInterface();
    mVCSTypes.clear();
}

VCSManager* VCSManager::getInstance()
{
    if (!mInstance)
    {
        mInstance = new VCSManager;
    }
    return  mInstance;
}

void VCSManager::destroyInstance()
{
    if (mInstance)
    {
        delete mInstance;
        mInstance = 0;
    }
}

bool VCSManager::registerVCSInterface(VCSInterface* interface)
{
    Q_ASSERT(interface != 0);
    QString interfaceName = interface->repoType();
    if (mVCSInterfacesMap.contains(interfaceName))
        return false;
    mVCSInterfacesMap[interfaceName] = interface;
    return true;
}

bool VCSManager::unregisterVCSInterface(VCSInterface* interface)
{
    Q_ASSERT(interface != 0);
    QString interfaceName = interface->repoType();
    if (!mVCSInterfacesMap.contains(interfaceName))
        return false;
    mVCSInterfacesMap.remove(interfaceName);
    delete interface;
    return true;
}

void VCSManager::unregisterAllVCSInterface()
{
    foreach (VCSInterface *interface, mVCSInterfacesMap)
    {
        delete interface;
        interface = 0;
    }
    mVCSInterfacesMap.clear();
}

bool VCSManager::unregisterVCSInterface(const QString& name)
{
    if (!mVCSInterfacesMap.contains(name))
        return false;
    mVCSInterfacesMap.remove(name);
    return true;
}

bool VCSManager::initEmptyRepo(const QString path, const QString &type, bool setAsWorkType)
{
    if (!mVCSInterfacesMap.contains(type))
        return false;
    if (!QDir(path).exists())
        return false;
    VCSInterface *interface = mVCSInterfacesMap.value(type);
    if (interface->initRepo(path) != VCSInterface::VCS_OK)
        return false;
    if (setAsWorkType)
        mWorkingRepoType = type;
    return true;
}

bool VCSManager::setWorkingRepoType(const QString& type)
{
    if (!mVCSInterfacesMap.contains(type))
        return false;
    mWorkingRepoType = type;
    return true;
}

VCSInterface* VCSManager::getWorkingRepoInterface() const
{
    return mVCSInterfacesMap.value(mWorkingRepoType);
}

QString VCSManager::detectRepoType(const QString& folderPath)
{
    if (!QDir(folderPath).exists())
        return "";
    foreach (QString type, mVCSTypes)
    {
        if (QDir(folderPath + "/." + type).exists())
            return type;
    }
    return "";
}

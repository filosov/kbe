#include "vcsinterface.hpp"
#include "vcsauthinterface.hpp"
#include "vcsfilestateinterface.hpp"
#include "vcstransactioninterface.hpp"
#include "vcspropertyinterface.hpp"
#include "VCSBranchInterface.hpp"

VCSInterface::VCSInterface()
    : mBranchInterface(0)
    , mFileStateInterface(0)
    , mTransactionInterface(0)
    , mPropertyInterface(0)
    , mAuthInterface(0)
{

}

VCSInterface::~VCSInterface()
{

}

VCSInterface::ResultType VCSInterface::addFile(const QString& path)
{

}

VCSInterface::ResultType VCSInterface::removeFile(const QString& path)
{

}

VCSInterface::ResultType VCSInterface::addFileToIgnoreList(const QString& path)
{

}

VCSInterface::ResultType VCSInterface::removeFileFromIgnoreList(const QString& path)
{

}

VCSInterface::ResultType VCSInterface::createBranch(const QString& name, const QString& fromCommit)
{
    if (!mBranchInterface)
        return VCS_NotImplemented;
    bool res = mBranchInterface->createBranch(name, fromCommit);
    return res ? VCS_OK : VCS_FAIL;
}

VCSInterface::ResultType VCSInterface::deleteBranch(const QString& name)
{
    if (!mBranchInterface)
        return VCS_NotImplemented;
    bool res = mBranchInterface->deleteBranch(name);
    return res ? VCS_OK : VCS_FAIL;
}

VCSInterface::ResultType VCSInterface::mergeBranch(const QString& name)
{
    if (!mBranchInterface)
        return VCS_NotImplemented;
    bool res = mBranchInterface->mergeBranch(name);
    return res ? VCS_OK : VCS_FAIL;
}

VCSInterface::ResultType VCSInterface::rebaseBranch(const QString& name)
{
    return VCS_NotImplemented;
}

VCSInterface::ResultType VCSInterface::switchBranch(const QString& name)
{
    if (!mBranchInterface)
        return VCS_NotImplemented;
    bool res = mBranchInterface->switchBranch(name);
    return res ? VCS_OK : VCS_FAIL;
}

VCSInterface::ResultType VCSInterface::commit(const QString& commitComment, const QStringList& fileList)
{

}

VCSInterface::ResultType VCSInterface::commit(const QString& commitComment, const QString& file)
{

}

VCSInterface::ResultType VCSInterface::revert(const QStringList& fileList)
{

}

VCSInterface::ResultType VCSInterface::revert(const QString& fileList)
{

}

VCSInterface::ResultType VCSInterface::update(const QString folderPath)
{

}

VCSInterface::ResultType VCSInterface::applyProperty(const QString& value, const QString& key, const QString& filePath)
{

}

VCSInterface::ResultType VCSInterface::removeProperty(const QString& key, const QString& path)
{

}


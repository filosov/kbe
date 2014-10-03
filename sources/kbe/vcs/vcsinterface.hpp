#ifndef __VCSINTERFACE_HPP__
#define __VCSINTERFACE_HPP__

#include <QString>

class VCSAuthInterface;
class VCSTransactionInterface;
class VCSFileStateInterface;
class VCSPropertyInterface;
class VCSBranchInterface;

class VCSInterface
{
public:
    enum ResultType
    {
        VCS_OK,
        VCS_FAIL,
        VCS_NotInstalled = 10,
        VCS_ClientNotFound = 50,
        VCS_RemoteNotConfigurable = 100,
        VCS_RemoteUnavailable,
        VCS_NotImplemented = 500,
        VCS_UnsupportedOperation
    };

    virtual ~VCSInterface();
    virtual ResultType initRepo(const QString& path) = 0;
    virtual ResultType addFile(const QString& path);
    virtual ResultType removeFile(const QString& path);
    virtual ResultType addFileToIgnoreList(const QString& path);
    virtual ResultType removeFileFromIgnoreList(const QString& path);
    virtual ResultType createBranch(const QString& name, const QString &fromCommit = "");
    virtual ResultType deleteBranch(const QString& name);
    virtual ResultType mergeBranch(const QString& name);
    virtual ResultType rebaseBranch(const QString& name);
    virtual ResultType switchBranch(const QString& name);
    virtual ResultType commit(const QString& commitComment, const QStringList& fileList);
    virtual ResultType commit(const QString& commitComment, const QString& file);
    virtual ResultType revert(const QStringList& fileList);
    virtual ResultType revert(const QString& fileList);
    virtual ResultType update(const QString folderPath = "");
    virtual ResultType applyProperty(const QString& value, const QString& key, const QString& filePath);
    virtual ResultType removeProperty(const QString& key, const QString& path);
    virtual const QString repoType() = 0;

protected:
    VCSBranchInterface *mBranchInterface;
    VCSFileStateInterface *mFileStateInterface;
    VCSTransactionInterface *mTransactionInterface;
    VCSPropertyInterface *mPropertyInterface;
    VCSAuthInterface *mAuthInterface;

    explicit VCSInterface();

};

#endif

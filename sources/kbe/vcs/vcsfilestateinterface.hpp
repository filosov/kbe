#ifndef __VCSFILESTATEINTERFACE_HPP__
#define __VCSFILESTATEINTERFACE_HPP__

#include <QStringList>

class VCSFileStateInterface
{
public:
    virtual ~VCSFileStateInterface() { }

    virtual bool addFile(const QString& fileName) = 0;
    virtual bool addFile(const QStringList& filesList) = 0;
    virtual bool addFileToCommit(const QString& fileName) = 0;
    virtual bool addFilesToCommit(const QStringList& filesList) = 0;
    virtual bool removeFileFromCommit(const QString& fileName) = 0;
    virtual bool removeFilesFromCommit(const QStringList& filesList) = 0;
    virtual bool commit() = 0;
    virtual bool revert() = 0;
    virtual bool revert(const QStringList& filesName) = 0;
};

#endif

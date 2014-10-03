#ifndef VCSBRANCHINTERFACE_HPP
#define VCSBRANCHINTERFACE_HPP

#include <QString>

class VCSBranchInterface
{
public:
    virtual ~VCSBranchInterface() { }
    virtual bool createBranch(const QString& branchName, const QString& commit = "") = 0;
    virtual bool deleteBranch(const QString& branchName) = 0;
    virtual bool mergeBranch(const QString& branchName) = 0;
    virtual bool switchBranch(const QString& branchName) = 0;
    virtual bool createTag(const QString& tagName, const QString& commit = "") = 0;
    virtual bool deleteTag(const QString& tagName) = 0;
};

#endif

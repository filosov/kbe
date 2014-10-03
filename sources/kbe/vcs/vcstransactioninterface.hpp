#ifndef __VCSTRANSACTIONINTERFACE_HPP__
#define __VCSTRANSACTIONINTERFACE_HPP__

#include <QStringList>

class VCSTransactionInterface
{
public:
    virtual ~VCSTransactionInterface() { }

    virtual bool push() = 0;
    virtual bool push(const QStringList& commitList) = 0;
    virtual bool pull() = 0;
    virtual void setRemoteUrl(const QString& url) = 0;
};

#endif

#ifndef __VCSAUTHINTERFACE_HPP__
#define __VCSAUTHINTERFACE_HPP__

#include <QString>

class VCSAuthInterface
{

public:
    virtual ~VCSAuthInterface() {}

    virtual bool authentication(const QString& password, const QString& login) = 0;
    virtual bool authentication(const QString& login) = 0;
    virtual void addAuthenticationInfo(const QString& key, const QString& url, const QString& password, const QString& login) = 0;
    virtual void updateAuthenticationInfo(const QString& url, const QString& password, const QString& login, const QString& key) = 0;

};

#endif

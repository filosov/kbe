#ifndef __VCSPROPERTYINTERFACE_HPP__
#define __VCSPROPERTYINTERFACE_HPP__

#include <QString>

class VCSPropertyInterface
{
public:
    virtual ~VCSPropertyInterface() { }

    virtual bool addProperty(const QString& value, const QString& key, const QString& path) = 0;
    virtual bool updateProperty(const QString& value, const QString& key, const QString& path) = 0;
    virtual bool removeProperty(const QString& key, const QString& path) = 0;
};

#endif

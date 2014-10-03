#ifndef __PROJECT_HPP__
#define __PROJECT_HPP__

#include "restitem.hpp"

class Project : public RESTItem
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getProjectName WRITE setProjectName)
    Q_PROPERTY(QString shortName READ getShortProjectName WRITE setShortProjectName)
    Q_PROPERTY(QString projectDescription READ getProjectDescription WRITE setProjectDescription)
public:
    explicit Project(QObject *parent = 0)
        : RESTItem(parent)
    {}

    explicit Project(const QString& _name, const QString& _shortName,
                     const QString& _descr, QObject *parent = 0)
        : RESTItem(parent)
        , name(_name)
        , shortName(_shortName)
        , projectDescription(_descr)
    {}

    virtual ~Project() {}

    void setProjectName(const QString& _projectName) { name = _projectName; }
    const QString& getProjectName() const { return name; }

    void setShortProjectName(const QString& _shortName) {shortName = _shortName;}
    const QString& getShortProjectName() const { return shortName; }

    void setProjectDescription(const QString& _description) { projectDescription = _description; }
    const QString& getProjectDescription() const { return projectDescription; }
private:
    QString name;
    QString shortName;
    QString projectDescription;
};

#endif // PROJECT_HPP

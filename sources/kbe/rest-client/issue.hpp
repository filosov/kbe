#ifndef __ISSUE_HPP__
#define __ISSUE_HPP__

#include "restitem.hpp"

class Issue : public RESTItem
{
    Q_OBJECT
    Q_PROPERTY(QString issueSummary READ getSummry WRITE setSummary)
    Q_PROPERTY(QString projectName READ getProjectName WRITE setProjectName)
    Q_PROPERTY(QString milestone READ getMilestone WRITE setMilestone)
    Q_PROPERTY(QString state READ getState WRITE setState)
    Q_PROPERTY(QString priority READ getPriority WRITE setPriority)
    Q_PROPERTY(QString type READ getType WRITE setType)
    Q_PROPERTY(QString description READ getDescription WRITE setDescription)

public:
    explicit Issue(QObject *parent = 0)
        : RESTItem(parent)
    {}

    explicit Issue(const QString& _summary,
                   const QString& _project,
                   const QString& _milestone,
                   const QString& _state,
                   const QString& _priority,
                   const QString& _type,
                   const QString& _description,
            QObject *parent = 0)
        : RESTItem(parent)
        , issueSummary(_summary)
        , projectName(_project)
        , milestone(_milestone)
        , state(_state)
        , priority(_priority)
        , type(_type)
        , description(_description)
    {}

    virtual ~Issue() {}

    void setProjectName(const QString& _name) { projectName = _name; }
    void setMilestone(const QString& _value) { milestone = _value; }
    void setState(const QString& _value) { state = _value; }
    void setPriority(const QString& _value) { priority = _value; }
    void setType(const QString& _value) { priority = _value; }
    void setDescription(const QString& _value) { description = _value; }
    void setSummary(const QString& _value) { issueSummary = _value; }

    const QString& getProjectName() const { return projectName; }
    const QString& getMilestone() const { return milestone; }
    const QString& getState() const { return state; }
    const QString& getPriority() const { return priority; }
    const QString& getType() const { return type; }
    const QString& getDescription() const { return description; }
    const QString& getSummry() const { return issueSummary; }
private:
    QString issueSummary;
    QString projectName;
    QString milestone;
    QString state;
    QString priority;
    QString type;
    QString description;
};

#endif // ISSUE_HPP

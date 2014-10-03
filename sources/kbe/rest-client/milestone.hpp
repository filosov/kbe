#ifndef __MILESTONE_HPP__
#define __MILESTONE_HPP__

#include "restitem.hpp"

class Milestone : public RESTItem
{
    Q_OBJECT
    Q_PROPERTY(QString projectName READ getProjectName WRITE setProjectName)
    Q_PROPERTY(QString version READ getVersion WRITE setVersion)
    Q_PROPERTY(QString startDate READ getStartDate WRITE setStartDate)
    Q_PROPERTY(QString finishDate READ getFinishDate WRITE setFinishDate)
public:
    explicit Milestone(QObject *parent = 0)
        : RESTItem(parent)
    {}

    explicit Milestone(const QString& _projectName,
                       const QString& _version,
                       const QString& _startDate,
                       const QString& _finishDate,
                       QObject *parent = 0)
        :RESTItem(parent)
        , projectName(_projectName)
        , version(_version)
        , startDate(_startDate)
        , finishDate(_finishDate)
    {}

    virtual ~Milestone() {}

    void setVersion(const QString& _version) { version = _version; }
    const QString& getVersion() const { return version; }

    void setStartDate(const QString& _date) { startDate = _date; }
    const QString& getStartDate() const { return startDate; }

    void setFinishDate(const QString& _date) { finishDate = _date; }
    QString getFinishDate() const { return finishDate; }

    void setProjectName(const QString& _name) { projectName = _name; }
    const QString& getProjectName() const { return projectName; }
private:
    QString projectName;
    QString version;
    QString startDate;
    QString finishDate;
};

#endif // MILESTONE_HPP

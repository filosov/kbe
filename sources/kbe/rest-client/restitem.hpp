#ifndef RESTITEM_HPP
#define RESTITEM_HPP

#include <QObject>

class RESTItem : public QObject
{
    Q_OBJECT
public:
    explicit RESTItem(QObject *parent = 0)
        : QObject(parent)
    {}
    virtual ~RESTItem() { }
};

#endif // RESTITEM_HPP

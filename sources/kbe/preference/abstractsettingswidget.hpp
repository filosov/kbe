#ifndef ABSTRACTSETTINGSWIDGET_HPP
#define ABSTRACTSETTINGSWIDGET_HPP

#include <QWidget>

class QDomNode;

class AbstractSettingsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AbstractSettingsWidget(QWidget *parent = 0)
        : QWidget(parent)
    {
    }

    virtual ~AbstractSettingsWidget() { }
    virtual QDomNode* serialize() = 0;
    virtual void deserialize(const QDomNode* node) = 0;
    virtual QString settingsGroup() const = 0;
    virtual void saveSettings() = 0;
};

#endif // ABSTRACTSETTINGSWIDGET_HPP

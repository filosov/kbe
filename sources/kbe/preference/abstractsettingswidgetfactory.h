#ifndef ABSTRACTSETTINGSWIDGETFACTORY_H
#define ABSTRACTSETTINGSWIDGETFACTORY_H

class QListWidgetItem;
class QWidget;

class AbstractSettingsWidgetFactory
{
public:
    explicit AbstractSettingsWidgetFactory() { }
    virtual ~AbstractSettingsWidgetFactory() { }

    virtual QListWidgetItem* listItem() const = 0;
    virtual QWidget* settingsWidget() const = 0;
};

#endif // ABSTRACTSETTINGSWIDGETFACTORY_H

#ifndef BUILDSETTINGSFACTORY_H
#define BUILDSETTINGSFACTORY_H

#include "abstractsettingswidgetfactory.h"

class BuildSettingsFactory : public AbstractSettingsWidgetFactory
{
public:
    explicit BuildSettingsFactory();
    ~BuildSettingsFactory();
    QListWidgetItem* listItem() const;
    QWidget* settingsWidget() const;
};

#endif // BUILDSETTINGSFACTORY_H

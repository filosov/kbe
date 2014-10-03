#ifndef TRACKERSETTINGSFACTORY_HPP
#define TRACKERSETTINGSFACTORY_HPP

#include "abstractsettingswidgetfactory.h"

class TrackerSettingsFactory : public AbstractSettingsWidgetFactory
{
public:
    explicit TrackerSettingsFactory();
    ~TrackerSettingsFactory();
    QListWidgetItem* listItem() const;
    QWidget* settingsWidget() const;
};

#endif // TRACKERSETTINGSFACTORY_HPP

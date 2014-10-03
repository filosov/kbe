#include "trackersettingsfactory.hpp"
#include "trackersettingswidget.hpp"

#include <QIcon>
#include <QListWidgetItem>

TrackerSettingsFactory::TrackerSettingsFactory()
{
}

TrackerSettingsFactory::~TrackerSettingsFactory()
{
}

QListWidgetItem *TrackerSettingsFactory::listItem() const
{
    QListWidgetItem *item = new QListWidgetItem(QIcon(":media/icons/task_window.png"), QObject::tr("Task"));
    return item;
}

QWidget *TrackerSettingsFactory::settingsWidget() const
{
    return new TrackerSettingsWidget;
}

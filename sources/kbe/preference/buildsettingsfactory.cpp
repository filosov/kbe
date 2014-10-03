#include "buildsettingsfactory.h"
#include "buildsettingswidget.h"
#include <QListWidgetItem>

BuildSettingsFactory::BuildSettingsFactory()
    : AbstractSettingsWidgetFactory()
{
}

BuildSettingsFactory::~BuildSettingsFactory()
{
}

QListWidgetItem *BuildSettingsFactory::listItem() const
{
    QListWidgetItem *item = new QListWidgetItem(QIcon(":media/icons/build_build.png"), QObject::tr("Build"));
    return item;
}

QWidget *BuildSettingsFactory::settingsWidget() const
{
    return new BuildSettingsWidget;
}

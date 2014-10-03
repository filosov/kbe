#ifndef BUILDSETTINGSWIDGET_H
#define BUILDSETTINGSWIDGET_H

#include "abstractsettingswidget.hpp"

class QLineEdit;
class QListWidget;
class QPushButton;

class BuildSettingsWidget : public AbstractSettingsWidget
{
    Q_OBJECT
public:
    explicit BuildSettingsWidget(QWidget *parent = 0);
    virtual ~BuildSettingsWidget();

    QDomNode* serialize();
    void deserialize(const QDomNode* node);

    QString settingsGroup() const { return "Build"; }

    void saveSettings();
private:
    QLineEdit *mBuilderPathEdit;
    QLineEdit *mBuilderCommandOptionsEdit;
    QListWidget *mExtensionsList;
    QPushButton *mAddButton;
    QPushButton *mRemoveButton;
private slots:
    void openBrowseDialog();
    void addExtensions();
    void removeExtension();
    void extensionSelectionChanged();
};

#endif // BUILDSETTINGSWIDGET_H

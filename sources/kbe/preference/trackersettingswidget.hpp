#ifndef TRACKERSETTINGSWIDGET_HPP
#define TRACKERSETTINGSWIDGET_HPP

#include "abstractsettingswidget.hpp"

class QLineEdit;
class QComboBox;
class QLabel;
class QPushButton;
class Project;

class TrackerSettingsWidget : public AbstractSettingsWidget
{
    Q_OBJECT
public:
    explicit TrackerSettingsWidget(QWidget *parent = 0);
    ~TrackerSettingsWidget();

    QDomNode* serialize();
    void deserialize(const QDomNode *node);
    QString settingsGroup() const { return "Tracker"; }
    void saveSettings();
private:
    QLineEdit *mLoginEdit;
    QLineEdit *mPasswordEdit;
    QLineEdit *mUrlEdit;
    QComboBox *mProjectList;
    QPushButton *mTestButton;
    QLabel *mSuccessLabel;
    QLabel *mFailLabel;
private slots:
    void testConnection();
    void loginResult(bool success);
    void projectListComplete(QList<Project*>& list, bool success);
    void projectChanged(int index);
};

#endif // TRACKERSETTINGSWIDGET_HPP

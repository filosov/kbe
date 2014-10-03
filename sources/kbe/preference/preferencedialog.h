#ifndef PREFERENCEDIALOG_H
#define PREFERENCEDIALOG_H

#include <QDialog>
#include <List>

class QStackedWidget;
class QListWidget;
class QListWidgetItem;
class AbstractSettingsWidgetFactory;
class QDomNode;

class PreferenceDialog : public QDialog
{
    Q_OBJECT
public:
    explicit PreferenceDialog(QWidget *parent = 0);
    virtual ~PreferenceDialog();
    void configureDialog();
    bool registerSettingsFactory(AbstractSettingsWidgetFactory* factory);
    bool unregisterSettignsFactory(AbstractSettingsWidgetFactory* factory);
    void unregisterAllFactories();
protected:
    void showEvent(QShowEvent *evt);
    void hideEvent(QHideEvent *evt);
private:
    void loadConfiguration();
    void writeConfigFile(const QList<QDomNode*>& nodeList);

    typedef QList<AbstractSettingsWidgetFactory*> SettingsFactoryList;
    typedef SettingsFactoryList::iterator SettingsFactoryListIterator;
    QListWidget *mSettingsPageList;
    QStackedWidget* mWorkSpace;
    SettingsFactoryList mRegisteredSettingsFactories;
signals:
    
public slots:
    
private slots:
    void changePage(QListWidgetItem *currentItem, QListWidgetItem *previousItem);
    void apply();
    void cancel();
    void ok();
};

#endif // PREFERENCEDIALOG_H

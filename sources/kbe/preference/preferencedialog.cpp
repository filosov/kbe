#include "preferencedialog.h"
#include "abstractsettingswidgetfactory.h"
#include "abstractsettingswidget.hpp"
#include "config.h"
#include <QListWidget>
#include <QStackedWidget>
#include <QGridLayout>
#include <QPushButton>

#include <QDomDocument>

#include <QDebug>

PreferenceDialog::PreferenceDialog(QWidget *parent) :
    QDialog(parent)
{
    QGridLayout *mainLayout = new QGridLayout;
    mSettingsPageList = new QListWidget(this);
    mSettingsPageList->setIconSize(QSize(32, 32));
    mSettingsPageList->setMaximumWidth(150);
    connect(mSettingsPageList, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(changePage(QListWidgetItem*,QListWidgetItem*)));
    mainLayout->addWidget(mSettingsPageList, 0, 0);
    mWorkSpace = new QStackedWidget(this);
    mainLayout->addWidget(mWorkSpace, 0, 1, 1, 6);

    QPushButton *buttonApply = new QPushButton(QIcon(), tr("Apply"), this);
    QPushButton *buttonCancel = new QPushButton(QIcon(), tr("Cancel"), this);
    QPushButton *buttonOk = new QPushButton(QIcon(), tr("Ok"), this);
    connect(buttonApply, SIGNAL(clicked()), this, SLOT(apply()));
    connect(buttonCancel, SIGNAL(clicked()), this, SLOT(cancel()));
    connect(buttonOk, SIGNAL(clicked()), this, SLOT(ok()));
    mainLayout->addWidget(buttonOk, 1, 3);
    mainLayout->addWidget(buttonCancel, 1, 4);
    mainLayout->addWidget(buttonApply, 1, 5);

    setLayout(mainLayout);
    setMinimumSize(800, 400);
    setWindowTitle(tr("Application settings"));
}

PreferenceDialog::~PreferenceDialog()
{
    unregisterAllFactories();
}

bool PreferenceDialog::registerSettingsFactory(AbstractSettingsWidgetFactory *factory)
{
    if (mRegisteredSettingsFactories.contains(factory))
        return false;
    mRegisteredSettingsFactories.push_back(factory);
    return true;
}

bool PreferenceDialog::unregisterSettignsFactory(AbstractSettingsWidgetFactory *factory)
{
    if (!mRegisteredSettingsFactories.contains(factory))
        return false;
    mRegisteredSettingsFactories.removeOne(factory);
    delete factory;
    return true;
}

void PreferenceDialog::unregisterAllFactories()
{
    SettingsFactoryListIterator it = mRegisteredSettingsFactories.begin();
    while (it != mRegisteredSettingsFactories.end())
    {
        delete (*it);
    }
    mRegisteredSettingsFactories.clear();
}

void PreferenceDialog::showEvent(QShowEvent *evt)
{
    loadConfiguration();
    QDialog::showEvent(evt);
}

void PreferenceDialog::hideEvent(QHideEvent *evt)
{
    QDialog::hideEvent(evt);
}

void PreferenceDialog::loadConfiguration()
{
    QFile f(Config::settingsFilePath);
    f.open(QFile::ReadOnly);
    QDomDocument doc;
    QString errMessage;
    int errRow = -1, errCol = -1;
    doc.setContent(f.readAll(), &errMessage, &errRow, &errCol);
    if (!errMessage.isEmpty())
    {
        qDebug() << "Settings parsing failed with message: " << errMessage;
        qDebug() << "Position: " << errRow << ", " << errCol;
        f.close();
        return;
    }
    QDomElement root = doc.documentElement();
    for (int i = 0; i < mWorkSpace->count(); ++i)
    {
        AbstractSettingsWidget *w = static_cast<AbstractSettingsWidget*>(mWorkSpace->widget(i));
        if (w)
        {
            QDomNode settingsNode = root.namedItem(w->settingsGroup());
            if (!settingsNode.isNull())
            {
                w->deserialize(&settingsNode);
                w->saveSettings();
            }
        }
    }
    f.close();
}

void PreferenceDialog::writeConfigFile(const QList<QDomNode *> &nodeList)
{
    QDomDocument doc;
    QDomProcessingInstruction header = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild( header );
    QDomElement root = doc.createElement("kbe_settings");
    doc.appendChild(root);
    foreach (QDomNode* node, nodeList)
    {
        if (node)
            root.appendChild(*node);
    }
    QFile f(Config::settingsFilePath);
    f.open(QFile::WriteOnly);
    f.write(doc.toByteArray());
    f.close();
}

void PreferenceDialog::changePage(QListWidgetItem *currentItem, QListWidgetItem *previousItem)
{
    if (currentItem == previousItem)
        return;
    mWorkSpace->setCurrentIndex(mSettingsPageList->row(currentItem));
}

void PreferenceDialog::apply()
{
    QList<QDomNode*> nodeList;
    for (int i = 0; i < mWorkSpace->count(); ++i)
    {
        AbstractSettingsWidget *w = static_cast<AbstractSettingsWidget*>(mWorkSpace->widget(i));
        if (w)
        {
            nodeList.append(w->serialize());
            w->saveSettings();
        }
    }
    writeConfigFile(nodeList);
    foreach (QDomNode* node, nodeList)
    {
        delete node;
    }
    nodeList.clear();
}

void PreferenceDialog::cancel()
{
    reject();
}

void PreferenceDialog::ok()
{
    apply();
    accept();
}

void PreferenceDialog::configureDialog()
{
    mSettingsPageList->clear();
    while (mWorkSpace->count() > 0)
    {
        QWidget *w = mWorkSpace->widget(0);
        mWorkSpace->removeWidget(w);
        delete w;
    }
    SettingsFactoryListIterator it = mRegisteredSettingsFactories.begin();
    while (it != mRegisteredSettingsFactories.end())
    {
        AbstractSettingsWidgetFactory *factory = *it;
        mSettingsPageList->addItem(factory->listItem());
        mWorkSpace->addWidget(factory->settingsWidget());
        ++it;
    }
    loadConfiguration();
}

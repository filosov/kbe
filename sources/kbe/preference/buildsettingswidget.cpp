#include "buildsettingswidget.h"
#include "application.hpp"

#include <QVBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QFileDialog>
#include <QDomElement>

BuildSettingsWidget::BuildSettingsWidget(QWidget *parent) :
    AbstractSettingsWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout;

    QGroupBox *pathBox = new QGroupBox(tr("Build profiles"), this);
    QGridLayout *pathBoxLayout = new QGridLayout;
    pathBoxLayout->addWidget(new QLabel(tr("Path to builder"), pathBox), 0, 0);
    mBuilderPathEdit = new QLineEdit(pathBox);
    pathBoxLayout->addWidget(mBuilderPathEdit, 0, 1);
    QPushButton *browseButton = new QPushButton(tr("Browse"), pathBox);
    connect(browseButton, SIGNAL(clicked()), this, SLOT(openBrowseDialog()));
    pathBoxLayout->addWidget(browseButton, 0, 2);

    pathBoxLayout->addWidget(new QLabel(tr("Additional builder options:"), pathBox), 1, 0);
    mBuilderCommandOptionsEdit = new QLineEdit(pathBox);
    pathBoxLayout->addWidget(mBuilderCommandOptionsEdit, 1, 1);
    pathBox->setLayout(pathBoxLayout);

    mainLayout->addWidget(pathBox);


    QGroupBox *extensionBox = new QGroupBox(tr("Sc-memory extensions"), this);
    mExtensionsList = new QListWidget(extensionBox);
    connect(mExtensionsList, SIGNAL(itemSelectionChanged()), this, SLOT(extensionSelectionChanged()));
    QGridLayout *extensionLayout = new QGridLayout;
    extensionLayout->addWidget(mExtensionsList, 0, 0, 5, 1);
    mAddButton = new QPushButton(QIcon(":media/icons/extension_add.png"), "", extensionBox);
    connect(mAddButton, SIGNAL(clicked()), this, SLOT(addExtensions()));
    extensionLayout->addWidget(mAddButton, 0, 1);
    mRemoveButton = new QPushButton(QIcon(":media/icons/extension_remove.png"), "", extensionBox);
    connect(mRemoveButton, SIGNAL(clicked()), this, SLOT(removeExtension()));
    extensionLayout->addWidget(mRemoveButton, 1, 1);
    extensionBox->setLayout(extensionLayout);

    mainLayout->addWidget(extensionBox);

    setLayout(mainLayout);
}

BuildSettingsWidget::~BuildSettingsWidget()
{
    delete mBuilderPathEdit;
    delete mBuilderCommandOptionsEdit;
    delete mExtensionsList;
    delete mAddButton;
    delete mRemoveButton;
}

QDomNode* BuildSettingsWidget::serialize()
{
    QDomDocument* doc = new QDomDocument;
    QDomElement root = doc->createElement(settingsGroup());
    doc->appendChild(root);

    QDomElement builderRoot = doc->createElement("builder_settings");
    QDomElement builderPath = doc->createElement("path");
    QDomCDATASection pathContent = doc->createCDATASection(mBuilderPathEdit->text());
    builderPath.appendChild(pathContent);
    QDomElement optionsNode = doc->createElement("aditional_options");
    QDomCDATASection optionsNodeContent = doc->createCDATASection(mBuilderCommandOptionsEdit->text());
    optionsNode.appendChild(optionsNodeContent);

    builderRoot.appendChild(builderPath);
    builderRoot.appendChild(optionsNode);

    QDomElement extNode = doc->createElement("builder_extensions");
    for (int i = 0; i < mExtensionsList->count(); ++i)
    {
        QDomElement ext = doc->createElement("extension");
        ext.setAttribute("path", mExtensionsList->item(i)->text());
        extNode.appendChild(ext);
    }

    root.appendChild(builderRoot);
    root.appendChild(extNode);

    return doc;
}

void BuildSettingsWidget::deserialize(const QDomNode *node)
{
    mExtensionsList->clear();
    QDomNode builderRootNode = node->namedItem("builder_settings");
    if (!builderRootNode.isNull())
    {
        QDomNode builderPath = builderRootNode.namedItem("path");
        if (!builderPath.isNull())
        {
            QString path = builderPath.firstChild().nodeValue();
            mBuilderPathEdit->setText(path);
        }
        QDomNode builderOptions = builderRootNode.namedItem("aditional_options");
        if (!builderOptions.isNull())
        {
            QString options = builderOptions.firstChild().nodeValue();
            mBuilderCommandOptionsEdit->setText(options);
        }
    }
    QDomNode extensionRootNode = node->namedItem("builder_extensions");
    if (!extensionRootNode.isNull())
    {
        QDomNodeList extensions = extensionRootNode.childNodes();
        for (int i = 0; i < extensions.size(); ++i)
        {
            QDomNode ext = extensions.item(i);
            QDomNode pathAttr = ext.attributes().namedItem("path");
            if (!pathAttr.isNull())
            {
                QString path = pathAttr.nodeValue();
                mExtensionsList->addItem(path);
            }
        }
    }
}

void BuildSettingsWidget::saveSettings()
{
    const QString prefix = settingsGroup() + "_";
    qApp->addSettings(prefix + "BuilderPath", QVariant(mBuilderPathEdit->text()));
    qApp->addSettings(prefix + "BuilderOptions", QVariant(mBuilderCommandOptionsEdit->text().split(" ")));
    QStringList extList;
    for (int i = 0; i < mExtensionsList->count(); ++i)
    {
        extList.append(mExtensionsList->item(i)->text());
    }
    qApp->addSettings(prefix + "BuilderExtensions", QVariant(extList));
}

void BuildSettingsWidget::openBrowseDialog()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Choose executable"),
                                                     "",
                                                     tr("Executable binary(*.exe)"));
    mBuilderPathEdit->setText(fileName);
}

void BuildSettingsWidget::addExtensions()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Choose extensions"),
                                                     "",
                                                     tr("Any file(*.*)"));
    for (int i = 0; i < mExtensionsList->count(); ++i)
    {
        QString str = mExtensionsList->item(i) ? mExtensionsList->item(i)->text() : "";
        if (fileNames.contains(str))
            fileNames.removeOne(str);
    }
    mExtensionsList->addItems(fileNames);
}

void BuildSettingsWidget::removeExtension()
{
    QList<QListWidgetItem*> selectedItems = mExtensionsList->selectedItems();
    foreach (QListWidgetItem* item, selectedItems)
    {
        delete item;
    }
}

void BuildSettingsWidget::extensionSelectionChanged()
{
    mRemoveButton->setEnabled(mExtensionsList->selectedItems().count() > 0);
}

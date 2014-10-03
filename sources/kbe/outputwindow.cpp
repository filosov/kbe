#include "outputwindow.h"

#include <QTextEdit>
#include <QPushButton>
#include <QGridLayout>

OutputWindow::OutputWindow(QWidget *parent) :
    QWidget(parent)
{
    QGridLayout *mainLayout = new QGridLayout;

    QPushButton *buttonClear = new QPushButton(QIcon(":/media/icons/output_clear.png"), "", this);
    buttonClear->setFixedSize(36, 36);
    connect(buttonClear, SIGNAL(clicked()), this, SLOT(clearOutput()));
    mainLayout->addWidget(buttonClear, 0, 10);

    mOutputLog = new QTextEdit(this);
    mainLayout->addWidget(mOutputLog, 1, 0, 1, 11);

    setLayout(mainLayout);

    setWindowTitle(tr("Build log"));
}

OutputWindow::~OutputWindow()
{
    delete mOutputLog;
}

void OutputWindow::addMessage(const QString &message)
{
    mOutputLog->append(message);
}

void OutputWindow::clearOutput()
{
    mOutputLog->clear();
}

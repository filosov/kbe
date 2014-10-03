#include "projectbuilder.hpp"
#include "outputwindow.h"
#include "application.hpp"

#include <QDebug>

ProjectBuilder* ProjectBuilder::mInstance = 0;

ProjectBuilder::ProjectBuilder(QObject *parent) :
    QObject(parent)
{
    mBuilderProcess = new QProcess;
    connect(mBuilderProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readStdout()));
    connect(mBuilderProcess, SIGNAL(readyReadStandardError()), this, SLOT(readStderr()));
    connect(mBuilderProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(startError(QProcess::ProcessError)));
    connect(mBuilderProcess, SIGNAL(started()), this, SLOT(startSuccess()));

    mOutputLog = new OutputWindow;
}

ProjectBuilder::~ProjectBuilder()
{
    delete mOutputLog;
}

void ProjectBuilder::build()
{
    mOutputLog->clearOutput();
    const QString file = qApp->settings("ProjectFile").toString();
    if (file.isEmpty())
        return;
    const QString command = qApp->settings("Build_BuilderPath").toString();
    QStringList args = qApp->settings("Build_BuilderOptions").toStringList();
    args.append(qApp->settings("ProjectFile").toString());
    qDebug() << command;
    qDebug() << args;
    mBuilderProcess->start(command, args);
}

void ProjectBuilder::clean()
{
    mOutputLog->clearOutput();
}

void ProjectBuilder::rebuild()
{
    clean();
    build();
}

void ProjectBuilder::stop()
{
    if (mBuilderProcess->state() != QProcess::NotRunning)
        mBuilderProcess->kill();
}

void ProjectBuilder::readStderr()
{
    QString message = mBuilderProcess->readAllStandardError();
    mOutputLog->addMessage(message);
}

void ProjectBuilder::readStdout()
{
    QString message = mBuilderProcess->readAllStandardOutput();
    mOutputLog->addMessage(message);
}

void ProjectBuilder::startError(QProcess::ProcessError err)
{
    const QString message = QString("Process failed to start: %1").arg(err);
    qDebug() <<message;
    mOutputLog->addMessage(message);
}

void ProjectBuilder::startSuccess()
{
    const QString message = QString("Process started");
    qDebug() <<message;
}

void ProjectBuilder::destroyInstance()
{
    if (mInstance)
    {
        delete mInstance;
        mInstance = 0;
    }
}

ProjectBuilder *ProjectBuilder::getInstance()
{
    if (!mInstance)
    {
        mInstance = new ProjectBuilder;
    }
    return mInstance;
}

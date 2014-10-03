#ifndef PROJECTBUILDER_HPP
#define PROJECTBUILDER_HPP

#include <QObject>
#include <QStringList>
#include <QProcess>

class OutputWindow;

class ProjectBuilder : public QObject
{
    Q_OBJECT
public:
    static ProjectBuilder* getInstance();
    static void destroyInstance();

    OutputWindow* outputLogWindow() const { return mOutputLog; }
public slots:
    void build();
    void clean();
    void rebuild();
    void stop();

signals:
    void buildFinished(int code);
private slots:
    void readStderr();
    void readStdout();
    void startError(QProcess::ProcessError err);
    void startSuccess();
private:
    explicit ProjectBuilder(QObject *parent = 0);
    ~ProjectBuilder();
    static ProjectBuilder* mInstance;

    QProcess *mBuilderProcess;
    QStringList mBuildOptions;
    QStringList mCleanOptions;

    OutputWindow *mOutputLog;
};

#endif // PROJECTBUILDER_HPP

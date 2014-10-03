#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <QApplication>
#include <QMap>
#include <QVariant>

#if defined(qApp)
#undef qApp
#endif
#define qApp (static_cast<Application *>(QApplication::instance()))

class Application : public QApplication
{
public:
    explicit Application(int argc, char *argv[]);
    ~Application();
    bool addSettings(const QString& key, const QVariant value, bool overrideExistance = true);
    const QVariant settings(const QString& key);

private:
    QMap<QString, QVariant> mApplicationSettings;
};

#endif // APPLICATION_HPP

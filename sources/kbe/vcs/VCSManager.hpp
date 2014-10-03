#ifndef __VCSMANAGER_HPP__
#define __VCSMANAGER_HPP__

#include <QObject>
#include <QStringList>
#include <QMap>

class VCSInterface;

class VCSManager : public QObject
{
    Q_OBJECT
public:
    static const QString GIT;
    static const QString SVN;
    static const QString MERCURIAL;

	static VCSManager* getInstance();
	static void destroyInstance();
	bool registerVCSInterface(VCSInterface* interface);
	bool unregisterVCSInterface(VCSInterface* interface);
	void unregisterAllVCSInterface();
	bool unregisterVCSInterface(const QString& name);
    bool initEmptyRepo(const QString path, const QString& type, bool setAsWorkType = false);
	bool setWorkingRepoType(const QString& type);
    VCSInterface* getWorkingRepoInterface() const;
	QString detectRepoType(const QString& folderPath);

private:
	QMap<QString, VCSInterface*> mVCSInterfacesMap;
    QStringList mVCSTypes;
    static VCSManager* mInstance;
	QString mWorkingRepoType;

    explicit VCSManager();
    virtual ~VCSManager();
};

#endif

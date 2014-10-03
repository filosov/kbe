#include "querymanager.hpp"
#include <QRegExp>
#include <QSqlDatabase>
#include <QSqlError>

//#include "applicationsettings.h"
//#include "databasesettings.h"

#include <QDebug>

QueryManager* QueryManager::mInstance = 0;

QueryManager::QueryManager(QSqlDatabase connection, QObject *parent) :
    QObject(parent),
    mIsTransactionProccessed(false)
{
    mQueryConnection = connection;
}

QueryManager::~QueryManager()
{
}

void QueryManager::createNamedQuery(QString name, QString query)
{
    mNamedQuery[name] = query;
}

QSqlQuery QueryManager::executeNamedQuery(QString name)
{
    QStringList args = QStringList();
    return executeNamedQuery(name, args);
}

QSqlQuery QueryManager::executeNamedQuery(QString name, QStringList args)
{
    qDebug()<<"query name: "<<name;
    qDebug()<<"query params: "<<args;
    if (!mNamedQuery.contains(name))
    {
        mLastError = tr("No vailable queries for current name");
        qDebug()<<"No vailable queries for current name: "<<name;
        return QSqlQuery();
    }
    QString query = mNamedQuery.value(name);
    qDebug()<<"query structure: "<<query;
    int queryargsCount = query.count(QRegExp("%\\d+"));
    if (queryargsCount != args.size())
    {
        mLastError = tr("Required parameter's number in query isn't equal to given parameter");
        qDebug()<<"Required parameter's number in query isn't equal to given parameter";
        return QSqlQuery();
    }
    QRegExp reg("%\\d+");
    int pos = 0;
    for (int i = 0; i < queryargsCount; ++i)
    {
        if ((pos = reg.indexIn(query, pos)) != -1)
        {
            query = query.replace(pos, reg.matchedLength(), args.at(i));
            ++pos;
        }
    }
    qDebug()<<query;
    QSqlQuery q = mQueryConnection.exec(query);
    mLastError = q.lastError().text();
    if (!q.isSelect() && !mIsTransactionProccessed)
        emit dataChanged();
    return q;
}

bool QueryManager::executeTransaction(QString name)
{
    if (!connection().transaction())
        return false;
    qDebug()<<"====================Transaction started====================";
    mIsTransactionProccessed = true;
    mIdVector.clear();
    QList<QPair<QString, bool> > queries = mNamedTransaction.values(name);
    for (int i = queries.size() - 1; i != -1; --i)
    {
        QPair<QString, bool> query = queries.at(i);
        // получаем параметры для данного запроса и добавляем id, которые были сгенерированы во время транзакции
        QStringList params = mQueryParam.value(query.first);
        QRegExp exp("%(\\d+)");
        for (int j = 0; j < params.size(); ++j)
            if (exp.indexIn(params.at(j), 0) != -1)
            {
                int idNum = exp.cap(1).toInt() - 1;
                if (idNum >=0 && idNum < mIdVector.size())
                    params[j] = QString::number(mIdVector.at(idNum));
                else
                    qDebug()<<"No id with such number: "<<idNum;
            }
        QSqlQuery q = executeNamedQuery(query.first, params);
        if(!q.isActive())
        {
            connection().rollback();
            mIsTransactionProccessed = false;
            mLastError = connection().lastError().text();
            qDebug()<<mLastError;
            qDebug()<<"====================Transaction finished====================";
            return false;
        }
        if (query.second)
            mIdVector.push_back(q.lastInsertId().toInt());
        qDebug()<<mIdVector;
        q.finish();
    }
    mIsTransactionProccessed = false;
    if (connection().commit())
    {
        emit dataChanged();
        qDebug()<<"====================Transaction finished====================";
        return true;
    }
    else
    {
        mLastError = connection().lastError().text();
        qDebug()<<mLastError;
        qDebug()<<"====================Transaction finished====================";
        return false;
    }
}

bool QueryManager::prepareQuery(QString queryName, QStringList args)
{
    if (!mNamedQuery.contains(queryName))
        return false;
    mQueryParam[queryName] = args;
    return true;
}

bool QueryManager::addQueryToTransacton(QString transactionName, QString queryName, bool needCallLastId)
{
    if (!mNamedQuery.contains(queryName))
    {
        mLastError = "No such query " + queryName;
        return false;
    }
    mNamedTransaction.insertMulti(transactionName, qMakePair(queryName, needCallLastId));
    return true;
}

bool QueryManager::addQueryToTransacton(QString transactionName, QString queryName, QString query, bool needCallLastId)
{
    mNamedQuery[queryName] = query;
    mNamedTransaction.insertMulti(transactionName, qMakePair(queryName, needCallLastId));
    return true;
}

QSqlQuery QueryManager::executeQuery(QString strQuery)
{
    return QSqlQuery(strQuery, mQueryConnection);
}

QSqlDatabase QueryManager::connection() const
{
    return mQueryConnection;
}

QString QueryManager::lastError() const
{
    return mLastError;
}

void QueryManager::setConnection(QSqlDatabase conn)
{
    mQueryConnection = conn;
}

QueryManager *QueryManager::instance()
{
//    if (!mInstance)
//        mInstance = new QueryManager(QSqlDatabase::database(ApplicationSettings::instance()->databaseSettings()->databaseConnectionName()));
    return mInstance;
}

void QueryManager::destroyInstance()
{
    if (mInstance)
    {
        delete mInstance;
        mInstance = 0;
    }
}


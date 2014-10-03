#include "baserestclient.hpp"
#include <QtNetwork/QNetworkAccessManager>

BaseRestClient::BaseRestClient(QObject *parent) :
    QObject(parent)
{
    mNetworkManager = new QNetworkAccessManager(this);
}

BaseRestClient::~BaseRestClient()
{
    delete mNetworkManager;
}

bool BaseRestClient::addReply(QNetworkReply *reply, BaseRestClient::OperationType type)
{
    if (mReplyQueue.contains(reply))
        return false;
    mReplyQueue[reply] = type;
    return true;
}

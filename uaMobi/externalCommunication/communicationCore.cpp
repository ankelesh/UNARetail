#include "communicationCore.h"
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QHostInfo>
#include "dataFormats/formats.h"
#ifdef DEBUG
#include "submodules/UNAQtCommons/debug/debugtrace.h"
#endif

#include "dataproviders/sqldataprovider.h"

communicationCore::communicationCore(QObject* parent)
	: QObject(parent), deviceName(), manager(new QNetworkAccessManager(this)),
	timeoutInterval(240000)
{

}
communicationCore* communicationCore::_instanse = Q_NULLPTR;
communicationCore* communicationCore::instanse()
{
	if (_instanse == Q_NULLPTR)
	{
		_instanse = new communicationCore();
	}
	return _instanse;
}

QNetworkReply* communicationCore::get(QString url)
{
#ifdef DEBUG
	detrace_NETREQSENT("commcore::get", url, "");
#endif
	return manager->get(QNetworkRequest(url));
}

QNetworkReply* communicationCore::get(QUrl url)
{
#ifdef DEBUG
	detrace_NETREQSENT("commcore::get", url.toString(), "");
#endif
	return manager->get(QNetworkRequest(url));
}

QNetworkReply* communicationCore::post(QNetworkRequest& request,const QByteArray& data)
{
#ifdef DEBUG
	detrace_NETREQSENT("commcore::get", request.url().toString(), QString::fromUtf8(data));
#endif
    return manager->post(request, data);
}

QNetworkReply *communicationCore::sendUnboundRequest(QString url, QNetworkAccessManager::Operation op, QByteArray * data)
{
    switch(op)
    {
    case (QNetworkAccessManager::PostOperation):
    {
        QNetworkRequest temp (QUrl::fromUserInput(url));
        return instanse()->post(temp, (data == Q_NULLPTR) ? QByteArray() : *data);
    }
    case QNetworkAccessManager::GetOperation:
        return instanse()->get(QUrl::fromUserInput(url));
    default:
        return Q_NULLPTR;
    }
}

QNetworkReply* communicationCore::sendGETRequest(QString url)
{
	return instanse()->get(url);
}

QNetworkReply* communicationCore::sendGETRequest(QUrl url)
{
	return instanse()->get(url);
}

QNetworkReply* communicationCore::sendPOSTRequest(QNetworkRequest& request, const QByteArray& data)
{
	return instanse()->post(request, data);
}

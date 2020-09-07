#pragma once
#include <QObject>


/*
	This class is responsible for sending repeating request to provided address. It handles everything 
	including parsing by itself and is easy-to-use - just create this and connect response.

*/

class QNetworkAccessManager;
class QNetworkReply;
class QTimer;
class AsyncRequestEngine : public QObject
{
	Q_OBJECT
protected:
	QNetworkReply* awaitedReply;
	QTimer* timeoutTimer;   
	QString coreLink;				//	basic part of the link. Leave it empty if not required.
	bool repeatable;				//	if true, request error sends another request

	//	clears previous reply and attaches new one
	bool _setReply(QNetworkReply*);
public:
	AsyncRequestEngine(QString CoreLink, bool repeat, QObject* parent = Q_NULLPTR);
	// drops previous reply and sends new request using provided link
	void clearAndRequest(QString linkTail);
	// clears previous reply
	void clear();
	// resets core link and timeout interval
	void reinit(QString CoreLink, int timeoutInterval);

protected slots:
	void responseArrived();
	void errorArrived();
	void timeout();
signals:
	// contains full response without attemts to parse
	void clearResponse(QString);
	// splits arrived response by chr 30
	void parsedResponse(QStringList);
	// sends code of error
	void errorResponse(int);
};
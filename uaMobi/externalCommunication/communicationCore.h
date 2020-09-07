#pragma once
#include <QObject>
#include <QNetworkRequest>



// This singletone class is used to incapsulate everything used for networking - 
// device name, global timeout variables, network manager
// and 

class QNetworkAccessManager;
class QNetworkReply;
class communicationCore : public QObject	
{
	Q_OBJECT
protected:
	QString deviceName;						//	obtained by getName()
	QNetworkAccessManager* manager;			//	used for establishing connection
	int timeoutInterval;
	static communicationCore* _instanse;
	communicationCore(QObject* parent = Q_NULLPTR);
public:
	static communicationCore* instanse();
	QNetworkReply* get(QString url);
	QNetworkReply* get(QUrl url);

	QNetworkReply* post(QNetworkRequest& request, QByteArray& data);
	QString& getDeviceName() {
		return deviceName;
	}
	int getTimeoutInterval() {
		return timeoutInterval;
	}
	static QNetworkReply* sendGETRequest(QString url);
	static QNetworkReply* sendGETRequest(QUrl url);
	static QNetworkReply* sendPOSTRequest(QNetworkRequest& request, QByteArray& data);
};

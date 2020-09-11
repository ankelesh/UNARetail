#pragma once
#include <QObject>
#include "Datacore/AbsEntity.h"
#include <QVector>
#include <QNetworkAccessManager>
#include <QTimer>
#include <QNetworkReply>
#include <QQueue>
class PrinterWrapper : public  QObject
{
    Q_OBJECT
private:
    enum PWOperation {NoOp, PWConnect, PWStatus, PWOpenReceipt, PWSetGood, PWSetPay, PWFinish };
    QNetworkReply* awaited;
    QTimer* timeoutTimer;
    QString deviceUrl;
    PWOperation currentOp;
    bool serverAvailable;
    QQueue<QSharedPointer<EntityList> > PrintQueue;
    QSharedPointer<EntityList> currentReceipt;
    double summToPay;
    int attemtsToReconnect;
    PrinterWrapper();
	static PrinterWrapper* _instance;

    void _processQueue();
    void _openReceipt();
    void _setGood();
    void _setPay();
    void _finishReceipt();
    void _cleanAwaited();
public:
	static PrinterWrapper& instance();
    static void init();
	bool printerReady();
	bool preparePrinter();
	bool printReceipt(QSharedPointer<QVector<Entity>> data);
protected slots:
    void timeout();
    void onPrinterResponse();
    void onConnectResponse();
    void onPrintProcessResponse();
    void onReceiptOpened();
    void onGoodSet();
    void onPaySet();
    void onReceiptFinished();
};

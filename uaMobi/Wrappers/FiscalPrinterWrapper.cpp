#include "FiscalPrinterWrapper.h"
#include "widgets/UtilityElements/ExtendedDialogs.h"
#ifdef DEBUG
#include "debugtrace.h"
#endif
#include <QNetworkReply>
#include "externalCommunication/communicationCore.h"
#include "widgets/utils/GlobalAppSettings.h"
#include "Datacore/DataEntities.h"
#include "Datacore/ProductEntity.h"
FiscalPrinterWrapper* FiscalPrinterWrapper::_instance = Q_NULLPTR;
FiscalPrinterWrapper::FiscalPrinterWrapper() : QObject(Q_NULLPTR), awaited(Q_NULLPTR),
timeoutTimer(new QTimer(this)),
deviceUrl("http://localhost:4444/"),
currentOp(PWConnect)
{
    if (!AppSettings->printOnlyToFile)
    {
        if (AppSettings->useComInsteadOfIp)
        {
            awaited = communicationCore::sendUnboundRequest(deviceUrl +
                QStringLiteral("Settings(com=%0,baud=9000,tcp=0,ip=,port=,password=%1)")
            .arg("COM" + QString::number(AppSettings->printerPort)).arg(AppSettings->printerPassword));
        }
        else
        {
            awaited = communicationCore::sendUnboundRequest(deviceUrl +
                QStringLiteral("Settings(com=,baud=,tcp=1,ip=%0,port=%1,password=%2)")
                .arg(AppSettings->printerIp).arg(AppSettings->printerPort).arg(AppSettings->printerPassword));
        }
        QObject::connect(awaited, &QNetworkReply::finished, this, &FiscalPrinterWrapper::onConnectResponse);
        QObject::connect(awaited, QOverload<>::of(&QNetworkReply::error), this, &FiscalPrinterWrapper::timeout);
    }
}

void FiscalPrinterWrapper::_processQueue()
{
    if (currentReceipt.isNull() && PrintQueue.isEmpty())
    {
        currentOp = NoOp;
        return;
    }
    switch (currentOp) {
    case NoOp:
    case PWOpenReceipt:
        _openReceipt(); break;
    case PWSetGood:
        _setGood(); break;
    case PWSetPay:
        _setPay(); break;
    case PWFinish:
        _finishReceipt(); break;
    default:
        break;
    }
}

void FiscalPrinterWrapper::_openReceipt()
{
    if (PrintQueue.isEmpty())
    {
        currentOp = NoOp;
        return;
    }
    currentOp = PWOpenReceipt;
    awaited = communicationCore::sendUnboundRequest(deviceUrl +
         QStringLiteral("OpenReceipt(OperNum=%0,OperPass=%1,OptionReceiptFormat=0,OptionFiscalRcpPrintType=2)")
           .arg(AppSettings->operatorNumber).arg(AppSettings->operatorPassword));
    QObject::connect(awaited, &QNetworkReply::finished, this, &FiscalPrinterWrapper::onReceiptOpened);
    QObject::connect(awaited, QOverload<>::of(&QNetworkReply::error), this, &FiscalPrinterWrapper::timeout);

}

void FiscalPrinterWrapper::_setGood()
{
    Product p(currentReceipt->last().staticCast<ProductEntity>());
    awaited = communicationCore::sendUnboundRequest(deviceUrl +
         QStringLiteral("SellPLUwithSpecifiedVAT(NamePLU=%0,OptionVATClass=%1,Price=%2,Quantity=%3,DiscAddP=%4,DiscAddV=%5,NamePLUextension=%6)")
           .arg(p->comment).arg("1").arg(p->price).arg(p->quantity).arg("00").arg("00").arg(""));
    summToPay += p->price * p->quantity;
    QObject::connect(awaited, &QNetworkReply::finished, this, &FiscalPrinterWrapper::onGoodSet);
    QObject::connect(awaited, QOverload<>::of(&QNetworkReply::error), this, &FiscalPrinterWrapper::timeout);
}

void FiscalPrinterWrapper::_setPay()
{
    awaited = communicationCore::sendUnboundRequest(deviceUrl +
         QStringLiteral("Payment(OptionPaymentType=%0,OptionChange=%1,Amount=%2,OptionChangeType=%3)")
           .arg("0").arg("1").arg(summToPay).arg("1"));
    QObject::connect(awaited, &QNetworkReply::finished, this, &FiscalPrinterWrapper::onPaySet);
    QObject::connect(awaited, QOverload<>::of(&QNetworkReply::error), this, &FiscalPrinterWrapper::timeout);
}

void FiscalPrinterWrapper::_finishReceipt()
{
    awaited = communicationCore::sendUnboundRequest(deviceUrl +
         QStringLiteral("CashPayCloseReceipt()"));
    QObject::connect(awaited, &QNetworkReply::finished, this, &FiscalPrinterWrapper::onReceiptFinished);
    QObject::connect(awaited, QOverload<>::of(&QNetworkReply::error), this, &FiscalPrinterWrapper::timeout);
}

void FiscalPrinterWrapper::_cleanAwaited()
{
    if (awaited != Q_NULLPTR)
    {
        awaited = Q_NULLPTR;
    }
}

FiscalPrinterWrapper& FiscalPrinterWrapper::instance()
{
	if (_instance == Q_NULLPTR)
	{
		_instance = new FiscalPrinterWrapper();
	}
    return *_instance;
}

void FiscalPrinterWrapper::init()
{
    if (_instance == Q_NULLPTR)
    {
        _instance = new FiscalPrinterWrapper();
    }
}

bool FiscalPrinterWrapper::printerReady()
{
    if (AppSettings->printOnlyToFile)
        return true;
    return serverAvailable;
}

bool FiscalPrinterWrapper::preparePrinter()
{
    currentOp = PWStatus;
    awaited = communicationCore::sendUnboundRequest(deviceUrl + "ReadVersion()");
    return serverAvailable;
}

bool FiscalPrinterWrapper::printReceipt(QSharedPointer<QVector<Entity>> data)
{
#ifdef DEBUG
    detrace_METHCALL("FiscalPrinterWrapper::printReceipt");
	for (EntityList::iterator ent = data->begin(); ent != data->end(); ++ent)
    {
		detrace_METHEXPL("printing entity of type " << (*ent)->myType() << " with data " << (*ent)->formatedView("|", ""));
    }
#endif
    if (AppSettings->printOnlyToFile)
    {
        QString receiptname = "/receipt_" + QDateTime::currentDateTime().toString("dd_MM_yyyy hh_mm_ss") + "_" + QString::number(AppSettings->operatorNumber) + ".txt";
        QFile outfile(AppSettings->toFilePrintFilepath + receiptname);
        outfile.open(QIODevice::WriteOnly);
        QTextStream text(&outfile);
        for (EntityList::iterator ent = data->begin(); ent != data->end(); ++ent)
        {
            text << (*ent)->receiptView();
        }
        text.flush();
        outfile.close();
        return true;
    }
    if (!serverAvailable)
        return false;
    PrintQueue.push_back(data);
    if (currentOp == NoOp)
    {
        _processQueue();
    }
    return true;
}

void FiscalPrinterWrapper::timeout()
{
    if (attemtsToReconnect > 5)
    {
        ErrorMessageDialog::showErrorInfo(tr("no connection"), tr("Can not connect for too much attempts. Stop attempts."));
        currentOp = NoOp;
        return;
    }
    if (awaited != Q_NULLPTR)
    {
        ErrorMessageDialog::showErrorInfo(tr("error"), tr("can not connect to printer, retrying"), false,
                                           QString::fromUtf8(awaited->readAll()));
    }
    else
    {
       ErrorMessageDialog::showErrorInfo(tr("error"), tr("can not connect to printer, retrying"), false);
    }
    _processQueue();

}

void FiscalPrinterWrapper::onPrinterResponse()
{
    QString resp = QString::fromUtf8(awaited->readAll());
    _cleanAwaited();

}

void FiscalPrinterWrapper::onConnectResponse()
{
#ifdef DEBUG
    detrace_NETRESPARR(QString::fromUtf8(awaited->readAll()), awaited->errorString(), 0);
#endif
    _cleanAwaited();
    currentOp = NoOp;
    serverAvailable = true;
}

void FiscalPrinterWrapper::onPrintProcessResponse()
{

}

void FiscalPrinterWrapper::onReceiptOpened()
{
#ifdef DEBUG
    detrace_NETRESPARR(QString::fromUtf8(awaited->readAll()), awaited->errorString(), 0);
#endif
    _cleanAwaited();
    if (PrintQueue.isEmpty())
    {
        currentOp = NoOp;
        return;
    }
    currentOp = PWSetGood;
    currentReceipt = PrintQueue.takeFirst();
    _setGood();
}

void FiscalPrinterWrapper::onGoodSet()
{
#ifdef DEBUG
    detrace_NETRESPARR(QString::fromUtf8(awaited->readAll()), awaited->errorString(), 0);
#endif
    _cleanAwaited();
    currentReceipt->takeLast();
    if (currentReceipt.isNull() || currentReceipt->isEmpty())
    {
        currentOp = PWSetPay;
        _setPay();
    }
    else
    {
        _setGood();
    }
}

void FiscalPrinterWrapper::onPaySet()
{
#ifdef DEBUG
    detrace_NETRESPARR(QString::fromUtf8(awaited->readAll()), awaited->errorString(), 0);
#endif
    currentOp = PWFinish;
    _cleanAwaited();
    _finishReceipt();
}

void FiscalPrinterWrapper::onReceiptFinished()
{
#ifdef DEBUG
    detrace_NETRESPARR(QString::fromUtf8(awaited->readAll()), awaited->errorString(), 0);
#endif
    _cleanAwaited();
    if (!PrintQueue.isEmpty())
    {
        _processQueue();
    }
    else
    {
        currentOp = NoOp;
    }
}

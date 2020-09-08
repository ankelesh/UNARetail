#include "PrinterWrapper.h"
#include "widgets/UtilityElements/ExtendedDialogs.h"
#ifdef DEBUG
#include "debugtrace.h"
#endif
#include <QNetworkReply>
#include "externalCommunication/communicationCore.h"
#include "widgets/utils/GlobalAppSettings.h"
PrinterWrapper* PrinterWrapper::_instance = Q_NULLPTR;

PrinterWrapper::PrinterWrapper() : QObject(Q_NULLPTR), awaited(Q_NULLPTR),
timeoutTimer(new QTimer(this)),
deviceUrl("http://localhost:4444/"),
currentOp(PWConnect)
{
    awaited = communicationCore::sendUnboundRequest(deviceUrl +
         QStringLiteral("Settings(com=,baud=,tcp=1,ip=%0,port=%1,password=%2)")
           .arg(AppSettings->printerIp).arg(AppSettings->printerPort).arg(AppSettings->printerPassword));
    QObject::connect(awaited, &QNetworkReply::finished, this, &PrinterWrapper::onConnectResponse);
    QObject::connect(awaited, QOverload<>::of(&QNetworkReply::error), this, &PrinterWrapper::timeout);
}

void PrinterWrapper::_processQueue()
{
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

void PrinterWrapper::_openReceipt()
{
    if (PrintQueue.isEmpty())
    {
        currentOp = NoOp;
        return;
    }
    currentOp = PWOpenReceipt;
    awaited = communicationCore::sendUnboundRequest(deviceUrl +
         QStringLiteral("OpenReceipt(OperNum=%0,OperPass=%1,OptionReceiptFormat=0,OptionFiscalRcpPrintType=2)")
           .arg("0101").arg(""));
    QObject::connect(awaited, &QNetworkReply::finished, this, &PrinterWrapper::onReceiptOpened);
    QObject::connect(awaited, QOverload<>::of(&QNetworkReply::error), this, &PrinterWrapper::timeout);

}

void PrinterWrapper::_setGood()
{

}

void PrinterWrapper::_setPay()
{

}

void PrinterWrapper::_finishReceipt()
{

}

PrinterWrapper& PrinterWrapper::instance()
{
	if (_instance == Q_NULLPTR)
	{
		_instance = new PrinterWrapper();
	}
    return *_instance;
}

void PrinterWrapper::init()
{
    if (_instance == Q_NULLPTR)
    {
        _instance = new PrinterWrapper();
    }
}

bool PrinterWrapper::printerReady()
{
    return serverAvailable;
}

bool PrinterWrapper::preparePrinter()
{
    currentOp = PWStatus;
    awaited = communicationCore::sendUnboundRequest(deviceUrl + "ReadVersion()");
    return serverAvailable;
}

bool PrinterWrapper::printReceipt(QSharedPointer<QVector<Entity>> data)
{
#ifdef DEBUG
    detrace_METHCALL("PrinterWrapper::printReceipt");
	for (EntityList::iterator ent = data->begin(); ent != data->end(); ++ent)
    {
		detrace_METHEXPL("printing entity of type " << (*ent)->myType() << " with data " << (*ent)->formatedView("|", ""));
    }
#endif
    if (!serverAvailable)
        return false;
    PrintQueue.push_back(data);
    if (currentOp == NoOp)
    {
        _processQueue();
    }
    return true;
}

void PrinterWrapper::timeout()
{
    if (currentOp == PWConnect)
    {
        if (awaited != Q_NULLPTR)
        {
            ErrorMessageDialog::showErrorInfo(tr("error"), tr("can not connect to printer"), false,
                                              QString::fromUtf8(awaited->readAll()));
        }
        else
        {
            ErrorMessageDialog::showErrorInfo(tr("error"), tr("can not connect to printer"), false);
        }
    }
}

void PrinterWrapper::onPrinterResponse()
{
    QString resp = QString::fromUtf8(awaited->readAll());

}

void PrinterWrapper::onConnectResponse()
{
    currentOp = NoOp;
    serverAvailable = true;
}

void PrinterWrapper::onPrintProcessResponse()
{

}

void PrinterWrapper::onReceiptOpened()
{
    if (PrintQueue.isEmpty())
    {
        currentOp = NoOp;
        return;
    }
    currentReceipt = PrintQueue.head();


}

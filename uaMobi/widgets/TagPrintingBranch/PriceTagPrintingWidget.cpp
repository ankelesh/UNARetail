#include "PriceTagPrintingWidget.h"
#include <QLabel>
#include "Wrappers/PrinterWrappers/PrinterWrapperFactory.h"



static QString defaultTemplate(
    QStringLiteral("! UF test.fmt\r\n%0 %2 %3\r\n")
);

PriceTagPrintingWidget::PriceTagPrintingWidget(QWidget* parent) : 
	SearchWidget(parent),
	connectionStatusLabel(new QLabel("no connection", this)),
    printerWrapper(PrinterWrapperFactory::fabricate(this))
{
	innerLayout->insertWidget(0, connectionStatusLabel);
#ifdef QT_VERSION5X
	QObject::connect(printerWrapper, &AbsPrinterWrapper::error, this, &PriceTagPrintingWidget::onWrapperError);
	QObject::connect(printerWrapper, &AbsPrinterWrapper::connected, this, &PriceTagPrintingWidget::onWrapperOk);
#else
	QObject::connect(printerWrapper, SIGNAL(error(QString)), this, SLOT(onWrapperError(QString)));
	QObject::connect(printerWrapper, SIGNAL(connected()), this, SLOT(onWrapperOk()));
#endif
	printerWrapper->establishConnection();
}

void PriceTagPrintingWidget::onWrapperOk()
{
	connectionStatusLabel->setText("OK!");
}

void PriceTagPrintingWidget::onWrapperError(QString error)
{
	connectionStatusLabel->setText("Error: " + error);
}

void PriceTagPrintingWidget::_emplaceBarcode(QString barcode, ShortBarcode info)
{
	SearchWidget::_emplaceBarcode(barcode, info);
	if (!info.isNull())
        printerWrapper->print(defaultTemplate.arg(info->info).arg(info->price).arg(info->discount));
}

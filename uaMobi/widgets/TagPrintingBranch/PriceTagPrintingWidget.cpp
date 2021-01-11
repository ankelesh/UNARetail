#include "PriceTagPrintingWidget.h"
#include <QLabel>
#include "Wrappers/PrinterWrappers/PrinterWrapperFactory.h"
#include "widgets/utils/GlobalAppSettings.h"
#include "widgets/SettingsWidgets/PrinterTemplatesSettings.h"
#include "widgets/utils/MegaIconButton.h"
#include "widgets/utils/ElementsStyles.h"


static QString defaultTemplate(
    QStringLiteral("! UF test.fmt\r\n{name} {price} {discount}\r\n")
);

PriceTagPrintingWidget::PriceTagPrintingWidget(QWidget* parent) : 
	SearchWidget(parent),
	connectionStatusLabel(new QLabel("no connection", this)),
    printerWrapper(PrinterWrapperFactory::fabricate(this)),
	printSettingButton(new MegaIconButton(this)),
	printSettings()
{
	innerLayout->insertWidget(0, connectionStatusLabel);
	buttonLayout->addWidget(printSettingButton);
	if (AppSettings->labelPrinterTemplateText.isEmpty())
		AppSettings->labelPrinterTemplateText = defaultTemplate;
	printSettingButton->setText(tr("settings"));
	printSettingButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);
	printSettingButton->setIcon(QIcon(":/res/settings.png"));
#ifdef QT_VERSION5X
	QObject::connect(printerWrapper, &AbsPrinterWrapper::error, this, &PriceTagPrintingWidget::onWrapperError);
	QObject::connect(printerWrapper, &AbsPrinterWrapper::connected, this, &PriceTagPrintingWidget::onWrapperOk);
	QObject::connect(printSettingButton, &MegaIconButton::clicked, this, &PriceTagPrintingWidget::settingsRequired);
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

QString PriceTagPrintingWidget::_prepareTemplate(ShortBarcode info)
{
	QString templ = AppSettings->labelPrinterTemplateText;
	templ.replace(
		"{price}", QString::number(info->price))
		.replace("{name}", info->info)
		.replace("{discount}", QString::number(info->discount));
	return templ;
}

void PriceTagPrintingWidget::_emplaceBarcode(QString barcode, ShortBarcode info)
{
	SearchWidget::_emplaceBarcode(barcode, info);
	if (!info.isNull())
        printerWrapper->print(_prepareTemplate(info));
}
void PriceTagPrintingWidget::settingsRequired()
{
	printSettings = new PrinterTemplatesSettings(this);

	QObject::connect(printSettings, &PrinterTemplatesSettings::backRequired, 
		this, &PriceTagPrintingWidget::settingsCloseRequired);
	_hideAndDeleteCurrent(printSettings);
}

void PriceTagPrintingWidget::settingsCloseRequired()
{
	_hideAndDeleteCurrent(untouchable);
}

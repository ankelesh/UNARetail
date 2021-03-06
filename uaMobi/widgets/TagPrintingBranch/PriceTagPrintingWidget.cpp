#include "PriceTagPrintingWidget.h"
#include <QLabel>
#include "submodules/UNAQtCommons/wrappers/DirectConnectionPrinters/PrinterWrapperFactory.h"
#include "widgets/utils/GlobalAppSettings.h"
#include "widgets/SettingsWidgets/PrinterTemplatesSettings.h"
#include "submodules/UNAQtCommons/widgets/UtilityElements/MegaIconButton.h"
#include "widgets/utils/ElementsStyles.h"


static QString defaultTemplate(
    QStringLiteral("! 10 200 200 350 1\n"
                   "LABEL\n"
                   "BAR-SENSE\n"
                   "BOX 1 0 560 120 1\n"
                   "ML 47\n"
                   "TEXT una14p.cpf 0 5 5 {name}\n"
                   "ENDML\n"
                   "TEXT una10p.cpf 0 5 95 {altname}\n"
                   "BOX 1 120 400 270 1\n"
                   "TEXT 0 2 40 130 1buc    Pret MDL\n"
                   "TEXT 4 3 50 155 {price}\n"
                   "BOX 400 120 560 270 1\n"
                   "TEXT 0 2 420 130 Pret unitar 1L\\kg\n"
                   "TEXT 4 1 430 150 {unitaryprice}\n"
                   "BOX 1 270 560 350 1\n"
                   "TEXT 0 3 2 280 {barcode}\n"
                   "TEXT 0 2 280 280 Pret din:\n"
                   "TEXT 0 2 380 280 !<DATE\n"
                   "B EAN13 1 1 22 14 310 {barcode}\n"
                   "TEXT 0 3 230 300 Ferma cu Origini SRL\n"
                   "TEXT 0 2 500 325 {code}\n"
                   "PRINT"
)
);

QString normalizeFloatString(QString fs)
{
    QString::iterator ch = fs.begin();

    int nullsToAppend = 0;
    bool was_dot = false;
    while (ch != fs.end())
    {
        if (*ch == '.')
        {
            nullsToAppend = 3 - (fs.end() - ch);
            was_dot = true;
            break;
        }
        ++ch;
    }
    if (nullsToAppend > 0)
        fs = fs.leftJustified(fs.count() + nullsToAppend, '0');
    if (!was_dot)
        fs+= ".00";
    return fs;
}

QString guessSymbology(QString bc)
{
    switch (bc.count())
    {
    default:
    case 13:
    case 14:
    case 5:
        return "EAN13";
    case 8:
        return "EAN8";
    }
}

QString normalizeBarcode(QString bc)
{
    switch (bc.count())
    {
    case 5:
        return AppSettings->extrasearchPrefix + bc + "000000";
    default:
        return bc;
    }

}

QString _prepare_long_string(QString str, int mlen)
{
    if (str.count() < mlen)
        return str;
    QString buffer;
    buffer.reserve(str.count() + 10);
    int cnt = 0;
    for (QString::iterator sym = str.begin(); sym != str.end(); ++sym)
    {
        if (cnt >= mlen)
        {
            buffer += "\r\n";
            cnt = 0;
        }
        buffer += *sym;
        ++cnt;
    }
    return buffer;
}

PriceTagPrintingWidget::PriceTagPrintingWidget(QWidget* parent) : 
	SearchWidget(parent),
    statusLayout(new QHBoxLayout(this)),
	connectionStatusLabel(new QLabel("no connection", this)),
    templateNameLabel(new QLabel(this)),
    printerWrapper(PrinterWrapperFactory::fabricate(this)),
	printSettingButton(new MegaIconButton(this)),
	printSettings()
{
	innerLayout->insertLayout(0, statusLayout);
    statusLayout->addWidget(connectionStatusLabel);
    statusLayout->addWidget(templateNameLabel);
    templateNameLabel->setAlignment(Qt::AlignRight);

	buttonLayout->addWidget(printSettingButton);
	printSettingButton->setText(tr("settings"));
	printSettingButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);
	printSettingButton->setIcon(QIcon(":/resources/settings"));
    usedTemplate = AppData->getTemplateByGUID(AppSettings->labelPrinterTemplateGUID);
    if (usedTemplate->isValid())
    {
        templateNameLabel->setText(usedTemplate->templateName);
    }
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
    if (!usedTemplate->isValid())
        return QString();
	QString templ = usedTemplate->templateText;
	templ.replace(
        "{price}", normalizeFloatString(QString::number(info->price)))
        .replace("{name}", _prepare_long_string(info->info, usedTemplate->width / 2.5))
        .replace("{discount}", normalizeFloatString(QString::number(info->discount)))
        .replace("{symbology}", guessSymbology(info->barcode))
        .replace("{barcode}", normalizeBarcode(info->barcode))
        .replace("{altname}", info->nameAlt)
        .replace("{unitaryprice}", normalizeFloatString(QString::number(info->unitPrice)))
        .replace("{code}", QString::number(info->code))
        .replace("\n", "\r\n");

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
    usedTemplate = AppData->getTemplateByGUID(AppSettings->labelPrinterTemplateGUID);
    if (usedTemplate->isValid())
    {
        templateNameLabel->setText(usedTemplate->templateName);
    }
    else
        templateNameLabel->clear();
    
}

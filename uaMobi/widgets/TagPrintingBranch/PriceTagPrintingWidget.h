#pragma once
#include "widgets/SearchBranch/SearchWidget.h"
#include "Datacore/PrinterTemplateEntity.h"

class AbsPrinterWrapper;
class PriceTagPrintingWidget : public SearchWidget
{
    Q_OBJECT
protected:
	QHBoxLayout* statusLayout;
	QLabel* connectionStatusLabel;
	QLabel* templateNameLabel;
	AbsPrinterWrapper* printerWrapper;
	MegaIconButton* printSettingButton;
	inframedWidget* printSettings;
	PrinterTemplate usedTemplate;

	QString _prepareTemplate(ShortBarcode info);
	virtual void _emplaceBarcode(QString barcode, ShortBarcode info) override;
public:
	PriceTagPrintingWidget(QWidget* parent = Q_NULLPTR);
protected slots:
	void onWrapperOk();
	void onWrapperError(QString error);
	void settingsRequired();
	void settingsCloseRequired();
};

#pragma once
#include "widgets/SearchBranch/SearchWidget.h"


class AbsPrinterWrapper;
class PriceTagPrintingWidget : public SearchWidget
{
    Q_OBJECT
protected:
	QLabel* connectionStatusLabel;
	AbsPrinterWrapper* printerWrapper;

	virtual void _emplaceBarcode(QString barcode, ShortBarcode info) override;
public:
	PriceTagPrintingWidget(QWidget* parent = Q_NULLPTR);
protected slots:
	void onWrapperOk();
	void onWrapperError(QString error);
};

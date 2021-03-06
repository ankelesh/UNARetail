#pragma once
#include "widgets/parents/AbstractScaningWidget.h"

class TwoLevelCounterLabel;
class SearchWidget : public AbstractScaningWidget
{
	Q_OBJECT
protected:
	using AbstractScaningWidget::_hideAndDeleteCurrent;
	using AbstractScaningWidget::untouchable;
	TwoLevelCounterLabel* priceCounter;
	TwoLevelCounterLabel* discountCounter;
	virtual void handleScanButton() override;		//	handles scan press
	virtual void handleValueFromKeyboard(QString value) override;
	virtual void barcodeReady() override;	
	virtual void _emplaceBarcode(QString barcode, ShortBarcode info) override;
#ifdef CAMERA_SUPPORT
	virtual void handleCameraBarcode(QString value) override;
#endif
	virtual void _clearControls() override;


public:
	virtual void show() override;
	SearchWidget(QWidget* parent);
};

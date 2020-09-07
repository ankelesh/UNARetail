#pragma once
#include "widgets/parents/AbstractScaningWidget.h"
/*
*/
class MegaIconButton;
class QuantityControl;
class TwoLevelCounterLabel;
class ScaningWidget :
	public AbstractScaningWidget
{
	Q_OBJECT
protected:
	TwoLevelCounterLabel* lengthInfo;					//	length of barcode
	TwoLevelCounterLabel* totalInfo;					//	how much quantity this barcode has already
	MegaIconButton* okButton;

	QuantityControl* quantityControl;
	QLabel* taxInvoiceInfo;
	QLineEdit* taxInvoiceField;
	Barcode pendingBarcode;

	virtual void _emplaceBarcode(QString barcode, ShortBarcode info) override;
	virtual void _clearControls() override;
	virtual void handleValueFromKeyboard(QString value) override;
	virtual void barcodeReady() override;
	virtual bool _validateBarcode(QString) override;
#ifdef CAMERA_SUPPORT
	virtual void handleCameraBarcode(QString value) override;
#endif
	virtual void handleScanButton();
public:
	ScaningWidget(Modes mode, QWidget* parent);

	void setTotal(int total);
	void setLen();
protected slots:					//	Slots for inner usage. To submit barcodes etc use tree intefaces
	void okPressed();
	// Inherited via AbstractScaningWidget
	//	drops backup and sends newBarcodeListCommite
};

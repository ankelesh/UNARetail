#pragma once
#include "widgets/parents/AbstractScaningWidget.h"
#include "Datacore/ShortBarcodeEntity.h"

namespace labels_private {
	class abs_counter_label;
}
class QuantityControl;
class QLineEdit;
class QLabel;
class MegaIconButton;
class DatabaseOperationsWidget : public AbstractScaningWidget
{
	Q_OBJECT
protected:
	QuantityControl* codeField;
	QuantityControl* countField;
	QuantityControl* priceField;
	QuantityControl* discountField;
	QHBoxLayout* operationPanel;
	MegaIconButton* addButton;
	MegaIconButton* removeButton;
	MegaIconButton* replaceButton;
	MegaIconButton* fullViewSwitcher;
	labels_private::abs_counter_label* totalBarcodes;
	ShortBarcode pendingEntity;
	ShortBarcode searchResult;

	int barcodesInBase;
	// Inherited via AbstractScaningWidget
	virtual void _emplaceBarcode(QString barcode, ShortBarcode info) override;
	virtual void _clearControls() override;
	virtual void handleValueFromKeyboard(QString value) override;
	virtual void barcodeReady() override;
    virtual void handleScanButton() override;
	void _clearEverything();
	void _setTotalBarcodes();
public:
	DatabaseOperationsWidget(QWidget* parent = Q_NULLPTR);

protected slots:
	void addPressed();
	void removePressed();
	void replacePressed();
	void fullViewRequested();



        // AbstractScaningWidget interface
protected slots:
        void handleCameraBarcode(QString value) override;
};

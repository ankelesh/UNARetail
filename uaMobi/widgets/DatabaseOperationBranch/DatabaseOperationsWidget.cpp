#include "DatabaseOperationsWidget.h"
#include "submodules/UNAQtCommons/widgets/ControlsWidgets/QuantityControl.h"
#include "submodules/UNAQtCommons/widgets/UtilityElements/MegaIconButton.h"
#include "submodules/UNAQtCommons/widgets/UtilityElements/ExtendedLabels.h"
#include "widgets/utils/ElementsStyles.h"

DatabaseOperationsWidget::DatabaseOperationsWidget(QWidget* parent)
	: AbstractScaningWidget(Modes::Search, parent),
	codeField(new QuantityControl(true, tr("code"),untouchable)),
	countField(new QuantityControl(false, tr("count"), untouchable)),
	priceField(new QuantityControl(false, tr("price"), untouchable)),
	discountField(new QuantityControl(false, tr("discount"), untouchable)),
	operationPanel(new QHBoxLayout(untouchable)),
	addButton(new MegaIconButton(tr("add"), untouchable)),
	removeButton(new MegaIconButton(tr("remove"), untouchable)),
	replaceButton(new MegaIconButton(tr("replace"), untouchable)),
	fullViewSwitcher(new MegaIconButton(tr("switchView"), untouchable)),
	totalBarcodes(new CounterLabel(untouchable)),
	pendingEntity(new ShortBarcodeEntity())
{
	controlLayout->addWidget(codeField->myWidget());
	controlLayout->addWidget(countField->myWidget());
	controlLayout->addWidget(priceField->myWidget());
	controlLayout->addWidget(discountField->myWidget());
	innerLayout->removeItem(buttonLayout);
	innerLayout->addItem(operationPanel);
	innerLayout->addItem(buttonLayout);
	operationPanel->addWidget(addButton);
	operationPanel->addWidget(removeButton);
	operationPanel->addWidget(replaceButton);
	operationPanel->addWidget(fullViewSwitcher);
	counterLayout->addWidget(totalBarcodes);

	addButton->setMaximumHeight(calculateAdaptiveButtonHeight(0.1));
	removeButton->setMaximumHeight(calculateAdaptiveButtonHeight(0.1));
	replaceButton->setMaximumHeight(calculateAdaptiveButtonHeight(0.1));
	fullViewSwitcher->setMaximumHeight(calculateAdaptiveButtonHeight(0.1));
	backButton->setMaximumHeight(calculateAdaptiveButtonHeight(0.1));
	keyboardButton->setMaximumHeight(calculateAdaptiveButtonHeight(0.1));
#ifdef CAMERA_SUPPORT
	cameraButton->setMaximumHeight(calculateAdaptiveButtonHeight(0.1));
#endif
	priceField->myWidget()->setMaximumHeight(calculateAdaptiveButtonHeight(0.1));
	countField->myWidget()->setMaximumHeight(calculateAdaptiveButtonHeight(0.1));
	discountField->myWidget()->setMaximumHeight(calculateAdaptiveButtonHeight(0.1));
	codeField->myWidget()->setMaximumHeight(calculateAdaptiveButtonHeight(0.1));

	totalBarcodes->setText(tr("total barcodes"));
	 codeField->show();
	countField->show();
	discountField->show();
	priceField->show();
	totalBarcodes->setMinimumHeight(calculateAdaptiveButtonHeight(0.08));
	codeField->setMaximum(9999999);

	QObject::connect(barcodeInfo, &ReturnableTextEdit::returnPressed, codeField, &abs_control::setFocus);
	QObject::connect(codeField, &abs_control::editingFinished, countField, &abs_control::setFocus);
	QObject::connect(countField, &abs_control::editingFinished, priceField, &abs_control::setFocus);
	QObject::connect(priceField, &abs_control::editingFinished, discountField, &abs_control::setFocus);
	QObject::connect(addButton, &MegaIconButton::clicked, this, &DatabaseOperationsWidget::addPressed);
	QObject::connect(removeButton, &MegaIconButton::clicked, this, &DatabaseOperationsWidget::removePressed);
	QObject::connect(replaceButton, &MegaIconButton::clicked, this, &DatabaseOperationsWidget::replacePressed);
	QObject::connect(fullViewSwitcher, &MegaIconButton::clicked, this, &DatabaseOperationsWidget::fullViewRequested);

	_setTotalBarcodes();
}

void DatabaseOperationsWidget::addPressed()
{
	barcodeReady();
	if (pendingEntity->barcode.isEmpty())
		return;
	if (searchResult.isNull())
	{

		AppData->pushIntoDownloaded(*pendingEntity);
	}
	else
	{
		AppData->removeFromDownloaded(searchResult);
		AppData->pushIntoDownloaded(*pendingEntity);
	}
	_clearEverything();
	_setTotalBarcodes();
}

void DatabaseOperationsWidget::removePressed()
{
	barcodeReady();
	if (pendingEntity->barcode.isEmpty())
		return;
	AppData->removeFromDownloaded(searchResult);
	_clearEverything();
	_setTotalBarcodes();
}

void DatabaseOperationsWidget::replacePressed()
{
	barcodeReady();
	if (pendingEntity->barcode.isEmpty())
		return;
	if (searchResult.isNull())
	{
		AppData->pushIntoDownloaded(*pendingEntity);
	}
	else
	{
		AppData->removeFromDownloaded(searchResult);
		AppData->pushIntoDownloaded(*pendingEntity);
	}
	_clearEverything();
	_setTotalBarcodes();
}

void DatabaseOperationsWidget::fullViewRequested()
{
}

void DatabaseOperationsWidget::_emplaceBarcode(QString barcode, ShortBarcode info)
{
	pendingEntity->clear();
	pendingEntity->barcode = barcode;
	if (info != Q_NULLPTR)
	{
		pendingEntity->info = info->info;
		barcodeInfo->setText(info->info);
		codeField->setEnumerableValue(info->code);
		priceField->setEnumerableValue(info->price);
		discountField->setEnumerableValue(info->discount);
		countField->setValue(info->count);
		codeField->setFocus();
		searchResult = info;
	}
	else
	{
		barcodeInfo->setFocus();
		barcodeInfo->clear();
		searchResult.clear();
	}
}

void DatabaseOperationsWidget::_clearControls()
{
}

void DatabaseOperationsWidget::handleValueFromKeyboard(QString value)
{
}

void DatabaseOperationsWidget::barcodeReady()
{
	pendingEntity->code = codeField->getPureValue();
	pendingEntity->count = countField->getPureValue();
	pendingEntity->discount = discountField->getPureValue();
	pendingEntity->price = priceField->getPureValue();
	pendingEntity->info = barcodeInfo->toPlainText();
    pendingEntity->GUID = (long long int)(barcodesInBase) + 1LL;
}

void DatabaseOperationsWidget::handleScanButton()
{

}

void DatabaseOperationsWidget::_clearEverything()
{
	barcodeInfo->clear();
	codeField->reset();
	priceField->reset();
	discountField->reset();
	countField->reset();
	barcodeInput->clear();
	pendingEntity->erase();
}

void DatabaseOperationsWidget::_setTotalBarcodes()
{
	barcodesInBase = AppData->countDownloaded();
	totalBarcodes->setValue(barcodesInBase);
}

void DatabaseOperationsWidget::handleCameraBarcode(QString value)
{
	hideCurrent();
    barcodeConfirmed(value);
}

#include "SearchWidget.h"
#include "widgets/utils/ElementsStyles.h"
#ifdef DEBUG
#include "submodules/UNAQtCommons/debug/debugtrace.h"
#endif
#include <QLabel>
#include "submodules/UNAQtCommons/widgets/UtilityElements/ExtendedLabels.h"
void SearchWidget::show()
{
	inframedWidget::show();
	barcodeInput->setFocus();
}
SearchWidget::SearchWidget(QWidget* parent)
	: AbstractScaningWidget( Modes::Search, parent), priceCounter(new TwoLevelCounterLabel(tr("Price"),
		std::nan(""),this)),
	discountCounter(new TwoLevelCounterLabel(tr("Discount"), std::nan(""), this))
{
	if (historyView != Q_NULLPTR)
		historyView->hide();
	barcodeInfo->setMinimumHeight(calculateAdaptiveButtonHeight(0.3));
	barcodeInfo->setMaximumHeight(calculateAdaptiveButtonHeight(0.8));
	barcodeInfo->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
	discountCounter->setMinimumHeight(calculateAdaptiveButtonHeight(0.1));

	QPalette whitenedBG = discountCounter->palette();
	whitenedBG.setColor(QPalette::Background, QColor(Qt::white));
	discountCounter->setPalette(whitenedBG);
	priceCounter->setPalette(whitenedBG);
	priceCounter->setAutoFillBackground(true);
	discountCounter->setAutoFillBackground(true);
	innerLayout->removeItem(additionalInputLayout);
	innerLayout->removeItem(buttonLayout);
	innerLayout->removeItem(counterLayout);
	innerLayout->removeWidget(barcodeInfo);
	innerLayout->removeItem(innerLayout->itemAt(innerLayout->count() - 1));
	innerLayout->addItem(counterLayout);
	innerLayout->addWidget(barcodeInfo);
	innerLayout->addLayout(additionalInputLayout);
	innerLayout->addLayout(buttonLayout);
	counterLayout->addWidget(priceCounter);
	counterLayout->addWidget(discountCounter);
}

void SearchWidget::handleScanButton()
{
	
}

void SearchWidget::handleValueFromKeyboard(QString value)
{
	barcodeConfirmed(value);
}

void SearchWidget::barcodeReady()
{

}

void SearchWidget::_emplaceBarcode(QString barcode, ShortBarcode e)
{
	barcodeInput->setText(barcode);
	if (e != Q_NULLPTR)
	{
		barcodeInfo->setText(e->formatedView("\n", DATE_ENCODING_FORMAT));
		priceCounter->setValue(e->price);
		discountCounter->setValue(e->discount);
	}
	barcodeInput->setFocus();
	barcodeInput->selectAll();
}
#ifdef CAMERA_SUPPORT
void SearchWidget::handleCameraBarcode(QString value)
{
	hideCurrent();
	update();
	qApp->processEvents();
	barcodeConfirmed(value);
}
#endif

void SearchWidget::_clearControls()
{

}

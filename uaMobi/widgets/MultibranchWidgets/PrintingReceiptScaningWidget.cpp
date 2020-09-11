#include "PrintingReceiptScaningWidget.h"
#include "widgets/utils/MegaIconButton.h"
#include "widgets/utils/ElementsStyles.h"
#include "Datacore/DataEntities.h"
#include "Wrappers/PrinterWrapper.h"
#include "Datacore/EntityQuickHash.h"
#include "Wrappers/androidservicewrapper.h"
#include "widgets/utils/GlobalAppSettings.h"
void PrintingReceiptScaningWidget::barcodeReady()
{
	blockSignals(true);
	RoleBoundScaningWidget::barcodeReady();
	blockSignals(false);
}

PrintingReceiptScaningWidget::PrintingReceiptScaningWidget(Modes mode, int counter_role, QVector<control_bind> bounds, Entity prototype, QWidget* parent)
	: RoleBoundScaningWidget(mode, counter_role, bounds, prototype, parent), 
	printAndExitButton(new MegaIconButton(this))
{
	buttonLayout->removeWidget(okButton);
	buttonLayout->addWidget(printAndExitButton);
	buttonLayout->addWidget(okButton);
	printAndExitButton->setText(tr("print_and_exit"));
	printAndExitButton->setStyleSheet(OK_BUTTONS_STYLESHEET);
	printAndExitButton->setIcon(QIcon(":/res/rec2.png"));
	if (barcodeModel == Q_NULLPTR)
	{
		barcodeModel = new DataEntityListModel(this);
	}
    PrinterWrapper::init();
	QObject::connect(printAndExitButton, &MegaIconButton::clicked, this,&PrintingReceiptScaningWidget::printAndAxitPressed);

}

void PrintingReceiptScaningWidget::printAndAxitPressed()
{
	QHash<EntityHash, Entity> uniques;
	EntityHash hash;
	for (int i = 0; i < barcodeModel->rowCount(); ++i)
	{
		hash = quickHash(barcodeModel->directAccessByFlatIndex(i));
		if (uniques.contains(hash))
		{
			uniques[hash]->concatenate(barcodeModel->directAccessByFlatIndex(i));
		}
		else
		{
			uniques.insert(hash, Entity(barcodeModel->directAccessByFlatIndex(i)->clone()));
		}
	}
	QSharedPointer<QVector<Entity>> valuesToPrint( new QVector<Entity>());
    valuesToPrint->reserve(uniques.size());
    for (QHash<EntityHash, Entity>::iterator pair = uniques.begin(); pair != uniques.end(); ++pair)
    {
        if (pair.value()->isValid())
        {
            valuesToPrint->push_back(pair.value());
        }
    }
	emit receiptDataFinished(valuesToPrint);
    PrinterWrapper::instance().printReceipt(valuesToPrint);
    if (AppSettings->sendToEmailByIntent || AppSettings->sendAsMessageByIntent)
    {
        QString receiptContent;
        QTextStream receiptOutput(&receiptContent);
        for (EntityList::iterator ent = valuesToPrint->begin(); ent < valuesToPrint->end(); ++ent )
        {
            receiptOutput << (*ent)->receiptView();
        }
        receiptOutput.flush();
        if (AppSettings->sendToEmailByIntent)
           AndroidServiceWrapper::instance().sendEmailIntent("Header of the receipt", receiptContent , AppSettings->emailDestinations);
        if (AppSettings->sendAsMessageByIntent)
            AndroidServiceWrapper::instance().sendMessageIntent(receiptContent);
    }
    okPressed();
}

void PrintingReceiptScaningWidget::okPressed()
{
	for (int i = 0; i < barcodeModel->rowCount(); ++i)
	{
		emit barcodeReceived(barcodeModel->directAccessByFlatIndex(i));
	}
	RoleBoundScaningWidget::okPressed();
}

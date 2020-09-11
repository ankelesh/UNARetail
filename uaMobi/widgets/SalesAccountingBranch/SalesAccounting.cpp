#include "SalesAccounting.h"
#include "widgets/MultibranchWidgets/PrintingReceiptScaningWidget.h"
#include "widgets/MultibranchWidgets/innerSettingsForm.h"
#include "widgets/MultibranchWidgets/ScanedStorageWidget.h"
#include <QLabel>
#include "Datacore/ProductEntity.h"
#include "Datacore/DataEntities.h"
#include <cmath>
#include "widgets/utils/MegaIconButton.h"
#include "widgets/SettingsWidgets/SpecialActionsSettings.h"
double summSpentMoney(Entity e, double previous)
{
	Product p = upcastEntity<ProductEntity>(e);
	if (p.isNull())
		return qQNaN();
	else
        if (qIsNaN(previous))
			return  p->price * p->quantity;
		else
			return previous + p->price * p->quantity;
}

void autoinsert_price(Entity e, ShortBarcode sb, QVector<abs_control*>& c, QVector<labels_private::abs_counter_label*>&, QTextEdit* cc)
{
	if (c.length() == 2 && !(sb.isNull()))
	{
		QString valueToSet = (sb->discount > 0) ? QString::number(sb->discount) : QString::number(sb->price);
		c.first()->setValue(valueToSet);
		c.last()->setFocus();
	}
	return;
}
void set_minimum_by_history(Entity current, EntityHash hash, QVector<abs_control*>& controls,
	DataEntityListModel* history, QVector<EntityHash>& hashes)
{
	if (controls.count() != 2 || hashes.isEmpty())
		return;
	QVector<EntityHash>::iterator hiter = hashes.begin();
	double summ;
	bool found =false;
	
	while (hiter != hashes.end())
	{
		if ((*hiter) == hash)
		{
			if (!found)
			{
				controls.at(0)->setValue(QString::number(history->directAccessByFlatIndex(hiter - hashes.begin())->getEnumerable(Roles::Product::price)));
				controls.at(0)->myWidget()->setDisabled(true);
				controls.at(1)->setFocus();
				found = true;
			}
			summ += history->directAccessByFlatIndex(hiter - hashes.begin())->getEnumerable(Roles::Product::quantity);
		}
		++hiter;
	}
	if (!found)
	{
		controls.at(0)->myWidget()->setDisabled(false);
	}
	if (summ > 0)
	{
		controls.at(1)->setEnumerableMinimum(-summ);
	}
	else
	{
		controls.at(1)->setEnumerableMinimum(0);
	}


}

bool set_price_comment_as_previous(Entity pendingBarcode,EntityHash hash, QVector<EntityHash>& hashes, DataEntityListModel* history, QVector<abs_control*>& controls)
{
	if (!controls.isEmpty())
	{
		controls.at(0)->myWidget()->setEnabled(true);
	}
	return true;

}

SalesAccountingBranchWidget::SalesAccountingBranchWidget(QWidget* parent)
    : BranchRootWidget(Modes::SalesAccounting, parent), bounds(), prototype(new ProductEntity()),
      extraSettings(), extraSettingsButton(new MegaIconButton(this))
{
	pageName->setText(tr("Sales Accounting"));
	bounds << control_bind(InputControlEntity::Decimals,  Roles::Product::price, false, tr("price"));
	bounds << control_bind(InputControlEntity::Float,  Roles::Product::quantity, false, tr("sold"));
    extraSettingsButton->setIcon(QIcon(":/res/settings2.png"));
    extraSettingsButton->setText(tr("Extra settings"));
    QObject::connect(extraSettingsButton, &MegaIconButton::clicked, this, &SalesAccountingBranchWidget::extraSettingsRequired);
    innerLayout->removeWidget(backButton);
    innerLayout->addWidget(extraSettingsButton);
    innerLayout->addWidget(backButton);

}

void SalesAccountingBranchWidget::extraSettingsRequired()
{
    extraSettings = new SpecialActionsSettings(this);
    QObject::connect(extraSettings, &SpecialActionsSettings::backRequired, this, &SalesAccountingBranchWidget::backRequire);
    _hideAndDeleteCurrent(extraSettings);
}

inframedWidget* SalesAccountingBranchWidget::_allocateSettings()
{
	innerSettingsForm* insf = new innerSettingsForm(currentMode, this);
#ifdef QT_VERSION5X
	QObject::connect(insf, &innerSettingsForm::backRequired, this, &BranchRootWidget::backRequire);
#else
	QObject::connect(insf, SIGNAL(backRequired()), this, SLOT(backRequire()));
#endif
	return insf;
}

inframedWidget* SalesAccountingBranchWidget::_allocateScaning()
{
	PrintingReceiptScaningWidget* scanwidget =
		new PrintingReceiptScaningWidget(currentMode, Roles::Product::quantity, bounds, prototype, this);
	scanwidget->addExtraCounter(tr("summ"), &summSpentMoney, 1);
	scanwidget->hideCounter(-2);
	scanwidget->hideCounter(-1);
	scanwidget->setPostSearchTrigger(autoinsert_price);
	scanwidget->setHistoryCheckTrigger(set_minimum_by_history);
#ifdef QT_VERSION5X
	QObject::connect(scanwidget, &inframedWidget::backRequired, this, &BranchRootWidget::backRequire);
	QObject::connect(scanwidget, &RoleBoundScaningWidget::barcodeReceived,
		this, &BranchRootWidget::newBarcodeArrived);
#else
	QObject::connect(scanwidget, SIGNAL(backRequired()), this, SLOT(backRequire()));
	QObject::connect(scanwidget, SIGNAL(barcodeReceived(Entity)),
		this, SLOT(newBarcodeArrived(Entity)));
#endif
	return scanwidget;
}

inframedWidget* SalesAccountingBranchWidget::_allocateStorage()
{
	ScanedStorageWidget* s = new ScanedStorageWidget(currentMode, this);
#ifdef QT_VERSION5X
	QObject::connect(s, &ScanedStorageWidget::backRequired, this, &BranchRootWidget::backRequire);
#else
	QObject::connect(s, SIGNAL(backRequired()), this, SLOT(backRequire()));
#endif
	return s;
}

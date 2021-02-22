#include "RoleBoundScaningWidget.h"
#include <QKeyEvent>
#include <algorithm>
#include "widgets/utils/ElementsStyles.h"
#include <QtCore/QTimer>
#include "widgets/utils/GlobalAppSettings.h"
#include <QInputMethod>
#ifdef DEBUG
#include "submodules/UNAQtCommons/debug/debugtrace.h"
#endif
#include <qmessagebox.h>
#include <cmath>
#include <QtCore/QHash>
#include "widgets/ControlsMiniwidgets/QuantityControl.h"
#include "submodules/UNAQtCommons/widgets/UtilityElements/ExtendedLabels.h"
#include "submodules/UNAQtCommons/widgets/UtilityElements/MegaIconButton.h"
#include <exception>
#include "widgets/ControlsMiniwidgets/ControlManager.h"
#include <Datacore/DataEntities.h>
#include "submodules/UNAQtCommons/wrappers/Sound/SoundEffectPlayer.h"
void null_post_search(Entity, ShortBarcode, QVector<abs_control*>&, QVector<labels_private::abs_counter_label*>&, QTextEdit*)
{
	return;
}
void null_history_check(Entity, EntityHash, QVector<abs_control*>&,
	DataEntityListModel*, QVector<EntityHash>&)
{
	return;
 }
bool null_preinsert_trigger(Entity, EntityHash, QVector<EntityHash>&, DataEntityListModel*, QVector<abs_control*>&)
{
	return true;
}

void RoleBoundScaningWidget::_emplaceBarcode(QString barcode, ShortBarcode info)
{
	if (!barcode.isEmpty())
	{
		pendingBarcode->erase();
		pendingBarcode->setWriteable(0, barcode);
		controlList.first()->setFocus();
		if (AppSettings->autoFillQuantity)
			controlList.first()->setValue("1");
		else
			controlList.first()->setValue("0");
		pst(pendingBarcode, info, controlList, counters, barcodeInfo);
		
		barcodeInput->setText(barcode);
		
		if (info != Q_NULLPTR)
		{
			if (info->price > 0)
			{
				barcodeInfo->setText(
					QString::number(info->price) + " "
					+ info->info
				);
			}
			else
				barcodeInfo->setText(info->info);
			pendingBarcode->setWriteable(Roles::common::comment, info->info);
			commentHash = qHash(barcodeInfo->toPlainText());
		}
		else
		{
			barcodeInfo->clear();
			commentHash = 0;
		}
		setLen();
		setTotal(AppData->sumAllFilteredIn(currentMode, barcode, BarcodeEntity::getEnumerableFieldIndex(), pendingBarcode, TableNames::Scanned));
		pendingBcHash = hashingFunction(pendingBarcode);
		if (barcodeModel != Q_NULLPTR)
		{
			hct(pendingBarcode,pendingBcHash, controlList, barcodeModel, historyUniqueTracking);
		}
	}
}

void RoleBoundScaningWidget::_clearControls()
{
	QVector<abs_control*>::iterator it = controlList.begin();
	while (it != controlList.end())
	{
		(*it)->reset();
		++it;
	}
}

void RoleBoundScaningWidget::handleValueFromKeyboard(QString val)
{
	if (barcodeInput->text().isEmpty())
	{
		barcodeInput->setText(val);
		hideCurrent();
		_emplaceBarcode(val, _barcodeSearch(val));
		_clearControls();
	}
	else
	{
		hideCurrent();
		QVector<abs_control*>::iterator it = controlList.begin();
		while (it != controlList.end())
		{
			if (!(*it)->canGiveValue())
			{
				(*it)->setValue(val);
				if ((*it) == controlList.last())
				{
					barcodeReady();
					barcodeInput->clear();
				}
				break;
			}
			++it;
		}
	}
}

void RoleBoundScaningWidget::barcodeReady()
{
	QVector<int>::iterator bind = roleBinds.begin();
	QVector<abs_control*>::iterator control = controlList.begin();
	while (bind != roleBinds.end())
	{
		switch ((*control)->myType())
		{
		case InputControlEntity::Int:
		case InputControlEntity::Float:
			pendingBarcode->setEnumerable(*bind, (*control)->getEnumerableValue());
			break;
		case InputControlEntity::String:
		case InputControlEntity::Date:
			pendingBarcode->setWriteable(*bind, (*control)->getValue());
			break;
		default:
			break;
		}
		++bind;
		++control;
	}
	if (AppSettings->getModeDescription(currentMode).isInsertingTaxInvoiceNumAllowed())
	{
		if (AppSettings->getModeDescription(currentMode).isForbiddenInsertingWithoutTaxInvoice()
			&& taxInvoiceField->text().isEmpty())
		{
			taxInvoiceField->setStyleSheet(ERROR_LINEEDIT_STYLESHEET);
			return;
		}
		pendingBarcode->setWriteable(Roles::Barcode::taxInvoice, taxInvoiceField->text());
		AppSettings->taxInvoiceTemporary = taxInvoiceField->text();
	}
	if (commentHash != 0)
	{
		if (commentHash != qHash(barcodeInfo->toPlainText()))
		{
			pendingBarcode->setWriteable(Roles::common::comment, barcodeInfo->toPlainText());
		}
	}
	else
		pendingBarcode->setWriteable(Roles::common::comment, barcodeInfo->toPlainText());
	if (pendingBarcode->isValid())
	{
		if (barcodeModel != Q_NULLPTR) {
			if (!pit(pendingBarcode, pendingBcHash, historyUniqueTracking, barcodeModel, controlList))
			{
				return;
			}
		}
		_pushToHistory(pendingBarcode);
        AppSounds->play(1);
		emit barcodeReceived(pendingBarcode);
		_fill_counters();
		_clearControls();
		if (AppSettings->clearScanBuffer)
		{
			pendingBarcode->erase();
			barcodeInput->clear();
			barcodeInfo->clear();
			totalInfo->clear();
			lengthInfo->clear();
			barcodeInput->setFocus();
		}
		else
		{
			QString tname = pendingBarcode->getName();
			QString tcomment = pendingBarcode->getWriteable(Roles::common::comment);
			pendingBarcode->erase();
			pendingBarcode->setWriteable(Roles::common::barcode, tname);
			pendingBarcode->setWriteable(Roles::common::comment, tcomment);
		}
	}
}

bool RoleBoundScaningWidget::_validateBarcode(QString bc)
{
	if (AppSettings->getModeDescription(currentMode).isInsertingTaxInvoiceNumAllowed() 
		&& pendingBarcode->myType() == int(barcodeUtil::barcodetypes::uniformBc))
	{
		bool startsWithAlpha = true;
		switch (bc.count())
		{
		default:
		case 2:
			startsWithAlpha &= bc.at(1).isLetter();
		case 1:
			startsWithAlpha &= bc.at(0).isLetter();
			break;
		case 0:
			startsWithAlpha = false;
		}
		if (startsWithAlpha)
		{
			taxInvoiceField->setText(bc);
			AppSettings->taxInvoiceTemporary = pendingBarcode->getEnumerable(Roles::Barcode::taxInvoice);
			if (AppSettings->getModeDescription(currentMode).isForbiddenInsertingWithoutTaxInvoice())
			{
				taxInvoiceField->setStyleSheet("");
			}
			return false;
		}
	}
	return AbstractScaningWidget::_validateBarcode(bc);
}
#ifdef CAMERA_SUPPORT
void RoleBoundScaningWidget::handleCameraBarcode(QString value)
{
	if (!pendingBarcode->getName().isEmpty() && !controlList.first()->canGiveValue())
	{
		controlList.first()->setValue("1");
	}
	barcodeReady();
	hideCurrent();
	barcodeConfirmed(value);
}
#endif
void RoleBoundScaningWidget::handleScanButton()
{
	barcodeReady();
}

void RoleBoundScaningWidget::_fill_counters()
{
	QVector<labels_private::abs_counter_label*>::iterator counter = counters.begin();
	QVector<CounterFillerFuction>::iterator trigger = operators.begin();
	while (counter != counters.end())
	{
		if (!(*counter)->isHidden())
		{
			(*counter)->setValue((*trigger)(pendingBarcode, (*counter)->getValue()));
		}
		++counter; ++trigger;
	}
}

void RoleBoundScaningWidget::_pushToHistory(Entity e)
{
	if (barcodeModel != Q_NULLPTR)
	{
		barcodeModel->appendDataEntity(e);
		historyUniqueTracking << quickHash(e);
	}

}

RoleBoundScaningWidget::RoleBoundScaningWidget(Modes mode, int counter_role, 
	QVector<control_bind> bounds, Entity prototype, QWidget* parent)
	:
	AbstractScaningWidget(mode, parent),
	lengthInfo(new TwoLevelCounterLabel(tr("LEN:"), qQNaN(), untouchable)),
	totalInfo(new TwoLevelCounterLabel(tr("TOTAL:"), qQNaN(), untouchable)),
	okButton(new MegaIconButton(untouchable)),
	controlList(),
	roleBinds(),
	taxInvoiceInfo((AppSettings->getModeDescription(mode).isInsertingTaxInvoiceNumAllowed()) ?
		(new QLabel(tr("tax invoice info"), this)) : Q_NULLPTR),
	taxInvoiceField(
		(AppSettings->getModeDescription(mode).isInsertingTaxInvoiceNumAllowed()) ?
		new QLineEdit(this) : Q_NULLPTR
	),
	pendingBarcode(prototype), counterRole(counter_role),
	counters(), operators(), pst(null_post_search), hct(null_history_check),
	pit(null_preinsert_trigger), commentHash(0), historyUniqueTracking(),hashingFunction(quickHash),
	pendingBcHash()

{
	counterLayout->addWidget(lengthInfo);
	counterLayout->addWidget(totalInfo);
	buttonLayout->addWidget(okButton);

	okButton->setIcon(QIcon(":/res/submit.png"));
	okButton->setStyleSheet(QString(OK_BUTTONS_STYLESHEET));

	totalInfo->show();
	lengthInfo->show();

	lengthInfo->setStyleSheet(COUNTERS_LABEL_STYLESHEET);
	totalInfo->setStyleSheet(COUNTERS_LABEL_STYLESHEET);
#ifdef Q_OS_WINCE
	lengthInfo->setFont(AppFonts->makeFont(1.5));
	totalInfo->setFont(AppFonts->makeFont(1.5));
#else
	lengthInfo->setFont(AppFonts->makeFont(2.5));
	totalInfo->setFont(AppFonts->makeFont(2.5));
#endif
	totalInfo->setMinimumHeight(calculateAdaptiveButtonHeight());
	lengthInfo->setMinimumHeight(calculateAdaptiveButtonHeight());


	if (!AppSettings->navigationElements)
	{
		okButton->hide();
	}

	if (AppSettings->getModeDescription(mode).isInsertingTaxInvoiceNumAllowed())
	{
		controlLayout->addWidget(taxInvoiceInfo);
		taxInvoiceInfo->setAlignment(Qt::AlignCenter);
		controlLayout->addWidget(taxInvoiceField);
		taxInvoiceField->setText(AppSettings->taxInvoiceTemporary);
	}

	if (bounds.isEmpty())
		throw std::length_error("No controls");
	QVector<control_bind>::iterator cbind = bounds.begin();
	while (cbind != bounds.end())
	{
		abs_control* new_control = fabricateControl(cbind->type, cbind->control_name, controlLayout, untouchable);
		if (controlList.isEmpty())
		{
			QObject::connect(barcodeInfo, &ReturnableTextEdit::returnPressed, new_control, &abs_control::setFocus);
		}
		else
		{
			QObject::connect(controlList.last(), &abs_control::editingFinished, new_control, &abs_control::setFocus);
		}
		roleBinds << cbind->role;
		controlList << new_control;
		new_control->myWidget()->setMaximumHeight(calculateAdaptiveButtonHeight(0.1));
		new_control->show();

		++cbind;
	}
#ifdef Q_OS_ANDROID
    QObject::connect(controlList.last(), &abs_control::editingFinished, qApp->inputMethod(),&QInputMethod::hide);
#endif
	if (barcodeModel != Q_NULLPTR)
		historyUniqueTracking.reserve(50);
#ifdef QT_VERSION5X
	QObject::connect(controlList.last(), &abs_control::editingFinished, this, &RoleBoundScaningWidget::barcodeReady);
	QObject::connect(okButton, &QPushButton::clicked, this, &RoleBoundScaningWidget::okPressed);
#else
	QObject::connect(okButton, SIGNAL(clicked()), this, SLOT(okPressed()));
#endif
}


void RoleBoundScaningWidget::addExtraCounter(QString explanation, CounterFillerFuction f, int type)
{
	switch (type)
	{
	case 0:
	default:
		counters << new TwoLevelCounterLabel(explanation, qQNaN(), this);
		counters.last()->setMinimumHeight(calculateAdaptiveButtonHeight(0.08));
		break;
	case 1:
		counters << new CounterLabel(this);
		counters.last()->setText(explanation);
		counters.last()->setMinimumHeight(calculateAdaptiveButtonHeight(0.08));
		break;
	}
	counterLayout->addWidget(counters.last());
	operators << f;
}

void RoleBoundScaningWidget::hideCounter(int which)
{
	switch (which)
	{
	case -2:
		lengthInfo->hide();
		break;
	case -1:
		totalInfo->hide();
		break;
	default:
		if (which > 0 && which < counters.length())
		{
			counters.at(which)->hide();
		}
	}
}

void RoleBoundScaningWidget::setPostSearchTrigger(PostSearchTrigger tr)
{
	pst = tr;
}

void RoleBoundScaningWidget::setHistoryCheckTrigger(HistoryCheckTrigger tr)
{
	hct = tr;
}

void RoleBoundScaningWidget::setPreInsertingTrigger(PreInsertingTrigger tr)
{
	pit = tr;
}

void RoleBoundScaningWidget::setTotal(int /*total*/)
{
	if (!totalInfo->isHidden())
	{
		totalInfo->setValue(AppData->sumAllFilteredIn(currentMode, pendingBarcode->getName(),
			pendingBarcode->getFieldNumberForRole(counterRole), pendingBarcode, TableNames::Scanned));
	}
}

void RoleBoundScaningWidget::setLen()
{
	if (!lengthInfo->isHidden())
	{
		lengthInfo->setValue(pendingBarcode->getName().length());
	}
}

void RoleBoundScaningWidget::okPressed()
{
	barcodeReady();
	if (!AppSettings->clearScanBuffer)
		emit backRequired();
}

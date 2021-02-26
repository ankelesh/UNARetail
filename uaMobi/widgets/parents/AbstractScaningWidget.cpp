#include "AbstractScaningWidget.h"
#ifdef QT_VERSION5X
#include <QScroller>
#endif
#include "widgets/utils/ElementsStyles.h"
#include <QtCore/QTimer>
#ifdef DEBUG
#include "submodules/UNAQtCommons/debug/debugtrace.h"
#endif
#include "widgets/utils/ZebraListItemDelegate.h"
#include "widgets/utils/GlobalAppSettings.h"
#include "submodules/UNAQtCommons/wrappers/Sound/SoundEffectPlayer.h"
#include "submodules/UNAQtCommons/widgets/UtilityElements/ExtendedLabels.h"
#include "widgets/utils/EventsAndFilters.h"
#include "widgets/MultibranchWidgets/ScaningCameraWidget.h"
#include "widgets/utils/VirtualBarcodeKeyboard.h"
#include "submodules/UNAQtCommons/widgets/UtilityElements/MegaIconButton.h"
#include "submodules/UNAQtCommons/barcodeHandling/BarcodeObserver.h"
#include "Datacore/DataEntities.h"

AbstractScaningWidget::AbstractScaningWidget(Modes mode, QWidget* parent)
	: inframedWidget(parent), abstractDynamicNode(new inframedWidget(this), new QVBoxLayout(this)),
	innerLayout(new QVBoxLayout(untouchable)), counterLayout(new QHBoxLayout(untouchable)),
	controlLayout(new QVBoxLayout(untouchable)),
	buttonLayout(new QHBoxLayout(untouchable)),
	additionalInputLayout(new QHBoxLayout(untouchable)),
	modeName(new QLabel(untouchable)),
#ifdef DEBUG
	//debugInfo(new QLabel(untouchable)),
#endif
	barcodeInfo(new ReturnableTextEdit(untouchable)),
	barcodeInput(new ReturnEatingLineEdit(untouchable)),
#ifdef Q_OS_ANDROID
	barcodeLayout(new QHBoxLayout(this)),
	switchKeyboardTypeButton(new MegaIconButton(this)),
	switchDecorator(new SwitchableIMDecorator(barcodeField)),
#endif

	backButton(new MegaIconButton(untouchable)),
	keyboardButton(new MegaIconButton(untouchable)),
#ifdef CAMERA_SUPPORT
	cameraButton(new MegaIconButton(untouchable)),
#endif
	barcodeModel(),
	historyView(),
	currentMode(mode)
{
	

	untouchable->setLayout(innerLayout);
	innerLayout->addWidget(modeName);
#ifdef DEBUG
	//innerLayout->addWidget(debugInfo);
	//debugInfo->setWordWrap(true);
#endif
#ifdef Q_OS_ANDROID
	innerLayout->addLayout(barcodeLayout);
	barcodeLayout->addWidget(barcodeField);
	barcodeLayout->addWidget(switchKeyboardTypeButton);
#else
	innerLayout->addWidget(barcodeInput);
#endif
	
	innerLayout->addWidget(barcodeInfo);
	innerLayout->addLayout(counterLayout);
	innerLayout->addLayout(controlLayout);

	if (AppSettings->getModeDescription(currentMode).isHistoryRequired())
	{
		barcodeModel = new DataEntityListModel(this);
		historyView = new QListView(this);
		historyView->setItemDelegate(new ShortZebraItemDelegate(historyView));
		historyView->setModel(barcodeModel);
		historyView->setFont(AppFonts->makeFont(0.8));
		historyView->setSelectionMode(QListView::NoSelection);
		historyView->setVerticalScrollMode(QAbstractItemView::ScrollMode::ScrollPerItem);
		innerLayout->addWidget(historyView);
#ifdef Q_OS_ANDROID
		QScroller::grabGesture(historyView, QScroller::LeftMouseButtonGesture);
#endif
	}
	else
	{
		innerLayout->addStretch(0);
	}

	innerLayout->addLayout(additionalInputLayout);
	innerLayout->addLayout(buttonLayout);
	
	
	buttonLayout->addWidget(backButton);
	additionalInputLayout->addWidget(keyboardButton);
#ifdef CAMERA_SUPPORT
	additionalInputLayout->addWidget(cameraButton);
	cameraButton->setIcon(QIcon(":/resources/camera"));
	QObject::connect(cameraButton, &QPushButton::clicked, this, &AbstractScaningWidget::cameraRequired);
#endif

	// Layouts are reversal so that new elements were pushed to left
	buttonLayout->setDirection(QBoxLayout::RightToLeft);

	// No any spacings or margins
	innerLayout->setSpacing(0);
	innerLayout->setContentsMargins(0, 0, 0, 0);
	counterLayout->setSpacing(0);
	counterLayout->setContentsMargins(0, 0, 0, 0);
	controlLayout->setSpacing(0);
	controlLayout->setContentsMargins(0, 0, 0, 0);
	// Set up view
	backButton->setIcon(QIcon(":/resources/back"));
	
	keyboardButton->setIcon(QIcon(":/resources/key"));

	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);

//	barcodeInfo->setStyleSheet(COUNTERS_LABEL_STYLESHEET);
	barcodeInfo->setFixedHeight(calculateAdaptiveHeight());
	barcodeInfo->setFont(AppFonts->makeFont(3.0));
	barcodeInfo->setWordWrapMode(QTextOption::WordWrap);
	barcodeInfo->setAlignment(Qt::AlignCenter);
	QPalette p;
	p.setColor(QPalette::Base, qApp->palette().background().color());
	barcodeInfo->setPalette(p);
	//barcodeInfo->setTextBackgroundColor(qApp->palette().background().color());
	modeName->setFont(AppFonts->makeFont(2.0));
	modeName->setAlignment(Qt::AlignCenter);
	modeName->setText(transformModeToString(currentMode));

	barcodeInput->setMinimumHeight(calculateAdaptiveHeight());
	barcodeInput->setFont(AppFonts->makeFont(3.0));
	barcodeInput->setAlignment(Qt::AlignCenter);

#ifdef Q_OS_ANDROID
	QVector<Qt::InputMethodHints> hints;
	hints.push_back(Qt::ImhNone);
	hints.push_back(Qt::ImhDigitsOnly);
	switchDecorator->initiate(hints, true);
	switchKeyboardTypeButton->setIcon(QIcon(":/resources/key"));
	switchKeyboardTypeButton->setMaximumHeight(barcodeField->maximumHeight());
	switchKeyboardTypeButton->setMinimumHeight(barcodeField->minimumHeight());
	switchKeyboardTypeButton->setMinimumWidth(calculateAdaptiveWidth(0.1));
	QObject::connect(switchKeyboardTypeButton, &QPushButton::clicked, switchDecorator, &SwitchableIMDecorator::nextIM);
#endif

	if (!AppSettings->additionalControlElements)
	{
#ifdef CAMERA_SUPPORT
		cameraButton->hide();
#endif
		keyboardButton->hide();
	}
	if (!AppSettings->navigationElements)
	{
		backButton->hide();
	}
	modeName->hide();
#ifdef DEBUG
	QTimer* dtimer(new QTimer(this));
	dtimer->setInterval(200);
	QObject::connect(dtimer, &QTimer::timeout, this, &AbstractScaningWidget::refreshDebugState);
	dtimer->start();
#endif
	BarcodeObs->activate();
#ifdef QT_VERSION5X
	QObject::connect(backButton, &QPushButton::clicked, this, &AbstractScaningWidget::backRequired);
	QObject::connect(keyboardButton, &MegaIconButton::clicked, this, &AbstractScaningWidget::keyboardRequired);
	
	QObject::connect(BarcodeObs, &BarcodeObserver::barcodeCaught, this, &AbstractScaningWidget::barcodeConfirmed);
	QObject::connect(BarcodeObs, &BarcodeObserver::escapeCaught, this, &AbstractScaningWidget::backRequired);
	QObject::connect(barcodeInput, &QLineEdit::returnPressed, this, &AbstractScaningWidget::manualConfirmed);
#else
	QObject::connect(backButton, SIGNAL(clicked()), this, SIGNAL(backRequired()));
	QObject::connect(keyboardButton, SIGNAL(clicked()), this, SLOT(keyboardRequired()));

	QObject::connect(BarcodeObs, SIGNAL(barcodeCaught(QString)), this, SLOT(barcodeConfirmed(QString)));
	QObject::connect(BarcodeObs, SIGNAL(escapeCaught()), this, SIGNAL(backRequired()));
	QObject::connect(barcodeInput, SIGNAL(returnPressed()), this, SLOT(manualConfirmed()));
#endif
}

void AbstractScaningWidget::show()
{
	if (AppSettings->additionalControlElements)
	{
		keyboardButton->show();
#ifdef CAMERA_SUPPORT
		cameraButton->show();
#endif
	}
	else
	{
		keyboardButton->hide();
#ifdef CAMERA_SUPPORT
		cameraButton->hide();
#endif
	}
	if (AppSettings->navigationElements)
	{
		backButton->show();
	}
	else
	{
		backButton->hide();
	}
	inframedWidget::show();
	BarcodeObs->activate();
}

void AbstractScaningWidget::hide()
{
	BarcodeObs->deactivate();
	inframedWidget::hide();
}

void AbstractScaningWidget::barcodeConfirmed(QString barcode)
{
	if (_validateBarcode(barcode)) 
	{
        AppSounds->play(0);
		barcode = _extractionCheck(barcode);
		_clearControls();
		_emplaceBarcode(barcode, _barcodeSearch(barcode));
	}
}
#ifdef CAMERA_SUPPORT
void AbstractScaningWidget::cameraRequired()
//	This slot creates new camera widget because only one camera widget can exist
{
	_hideAndDeleteCurrent(new ScaningCameraWidget(this));
	QObject::connect(_upCO<ScaningCameraWidget>(), &ScaningCameraWidget::backRequired, this, &AbstractScaningWidget::hideCurrent);
	QObject::connect(_upCO<ScaningCameraWidget>(), &ScaningCameraWidget::hasBarcode, this, &AbstractScaningWidget::handleCameraBarcode);
}
#endif
#ifdef DEBUG
void AbstractScaningWidget::refreshDebugState()
{
//	debugInfo->setText(barcodeInfo->debugLine);
}
#endif
void AbstractScaningWidget::keyboardRequired()
//	This slot creates new keyboard
{
	_hideAndDeleteCurrent(new VirtualBarcodeKeyboard(this));
#ifdef QT_VERSION5X
	QObject::connect(_upCO<VirtualBarcodeKeyboard>(), &VirtualBarcodeKeyboard::backRequired, this, &AbstractScaningWidget::hideCurrent);
	QObject::connect(_upCO<VirtualBarcodeKeyboard>(), &VirtualBarcodeKeyboard::barcodeConfirmed, this, &AbstractScaningWidget::barcodeConfirmed);
	QObject::connect(_upCO<VirtualBarcodeKeyboard>(), &VirtualBarcodeKeyboard::valueConfirmed, this, &AbstractScaningWidget::handleValueFromKeyboard);
#else
	QObject::connect(_upCO<VirtualBarcodeKeyboard>(), SIGNAL(backRequired()), this, SLOT(hideCurrent()));
	QObject::connect(_upCO<VirtualBarcodeKeyboard>(), SIGNAL(barcodeConfirmed(QString)), this, SLOT(barcodeConfirmed()));
	QObject::connect(_upCO<VirtualBarcodeKeyboard>(), SIGNAL(valueConfirmed(QString)), this, SLOT(handleValueFromKeyboard()));
#endif
}

void AbstractScaningWidget::hideCurrent()
{
	_hideAndDeleteCurrent(untouchable);
}

void AbstractScaningWidget::manualConfirmed()
{
	barcodeConfirmed(barcodeInput->text());
}
QString AbstractScaningWidget::transformModeToString(Modes m)
{
	switch (m)
	{
	case Modes::Search:
		return tr("Search");
	case Modes::Inventory:
		return tr("Inventory");
	case Modes::Supplies:
		return tr("Supplies");
	case Modes::Prices:
		return tr("Prices");
	case Modes::Simple:
		return tr("Simple");
	default:
		return "";
	}
}

void AbstractScaningWidget::_pushToHistory(Entity barcode)
{
	if (barcodeModel != Q_NULLPTR)
	{
		barcodeModel->addToDataEntity(barcode, 0);
	}
}

bool AbstractScaningWidget::_validateBarcode(QString barcode)
{
	return !barcode.isEmpty();
}

QString AbstractScaningWidget::_extractionCheck(QString barcode)
{
	if (AppSettings->extrasearchPrefix.isEmpty())
		return barcode;
	else
	{
		if (barcode.startsWith(AppSettings->extrasearchPrefix))
		{
			QString::const_iterator point = barcode.begin() + AppSettings->extrasearchPrefix.count();
			bool allzeros = true;
			int shuffle = 0;
			while (point != barcode.end() && shuffle < 5)
			{
				QChar sym = (*(point + shuffle++));
				allzeros &= (sym == '0');
			}
			QString buffer;
			if (allzeros)
			{
				buffer.reserve(10);
				point += 5;
				while (point < barcode.end())
				{
					buffer += (*(point++));
				}
			}
			else
			{
				buffer.resize(5);
				std::copy(point, point + 5, buffer.begin());
			}
			if (!buffer.isEmpty())
			{
				return buffer;
			}
			return barcode;
			
		}
		else
		{
			return barcode;
		}
	}
}

ShortBarcode AbstractScaningWidget::_barcodeSearch(QString barcode)
{
	if (AppSettings->autoSearch)
	{
		return upcastEntity<ShortBarcodeEntity>(AppData->barcodeInfo(barcode));
	}
	return ShortBarcode();
}

void AbstractScaningWidget::focusInEvent(QFocusEvent* event)
{
	inframedWidget::focusInEvent(event);
	if (AppSettings->scanPrefix == 0)
		barcodeInput->setFocus();
}

bool checkBarcodeIntegrity(const QString& bc)
//	Realization of checking. No null barcodes, no too long
{
	if (bc.isEmpty() || bc.length() > 100)
	{
		return false;
	}
	return true;
}

void ReturnableTextEdit::focusInEvent(QFocusEvent* fev)
{
	QTextEdit::focusInEvent(fev);
    qApp->inputMethod()->show();
}

void ReturnableTextEdit::keyPressEvent(QKeyEvent* kev)
{
	if (kev->key() == Qt::Key_Return && this->hasFocus())
	{
		debugLine += "RPE";
		kev->accept();
		return;
	}
	QTextEdit::keyPressEvent(kev);
}

void ReturnableTextEdit::keyReleaseEvent(QKeyEvent* kev)
{
	if (kev->key() == Qt::Key_Return && this->hasFocus())
	{
		debugLine += "RRS";
		emit returnPressed();
		kev->accept();
		return;
	}
	else
		QTextEdit::keyReleaseEvent(kev);
}

ReturnableTextEdit::ReturnableTextEdit(QWidget* parent)
	:QTextEdit(parent)
{
	debugLine.reserve(200);
}

void ReturnEatingLineEdit::keyPressEvent(QKeyEvent* kev) 
{
	if (kev->key() == Qt::Key_Return)
	{
		kev->accept();
		return;
	}
	QLineEdit::keyPressEvent(kev);
}

void ReturnEatingLineEdit::keyReleaseEvent(QKeyEvent* kev)
{
	if (kev->key() == Qt::Key_Return)
	{
		emit returnPressed();
		kev->accept();
		return;
	}
	else
		QLineEdit::keyReleaseEvent(kev);
}

#include "BranchRootWidget.h"
#include "widgets/utils/ElementsStyles.h"
#include "widgets/MultibranchWidgets/innerSettingsForm.h"
#include "widgets/MultibranchWidgets/ScaningWidget.h"
#include "widgets/MultibranchWidgets/ScanedStorageWidget.h"
#include "widgets/MultibranchWidgets/SendingDataPickerWidget.h"
#include <QLabel>
#include "widgets/utils/MegaIconButton.h"
#include "dataproviders/BackupingEngine.h"
#ifdef DEBUG
#include "debugtrace.h"
#endif

BranchRootWidget::BranchRootWidget(Modes mode, QWidget* parent) :
	inframedWidget(parent),abstractDynamicNode(new inframedWidget(this), new QVBoxLayout(this)),
	innerLayout(new QVBoxLayout(untouchable)),
	pageName(new QLabel(this)),
	scanner(new MegaIconButton(untouchable)), scanned(new MegaIconButton(untouchable)),
	receive(new MegaIconButton(untouchable)), settings(new MegaIconButton(untouchable)),
	backButton(new MegaIconButton(untouchable)),
	currentMode(mode)
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(untouchable);
	untouchable->setLayout(innerLayout);
	innerLayout->addWidget(pageName);
	innerLayout->addWidget(scanner);
	innerLayout->addWidget(scanned);
	innerLayout->addWidget(receive);
	innerLayout->addWidget(settings);
	innerLayout->addWidget(backButton);

	mainLayout->setSpacing(0);		//	spacing removed to avoid space loss
	mainLayout->setContentsMargins(0, 0, 0, 0);

	innerLayout->setSpacing(0);
	innerLayout->setContentsMargins(0, 0, 0, 0);
	
	scanner->setIcon(QIcon(":/res/scanner.png"));
	scanner->setText(tr("branch_root_Scanner"));
	scanned->setIcon(QIcon(":/res/logs.png"));
	scanned->setText(tr("branch_root_Scanned"));
	receive->setIcon(QIcon(":/res/upload.png"));
	receive->setText(tr("branch_root_Send"));
	backButton->setIcon(QIcon(":/res/back.png"));
	backButton->setText(tr("branch_root_Back"));
	settings->setIcon(QIcon(":/res/settings.png"));
	settings->setText(tr("Settings"));
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);

	pageName->setFont(AppFonts->makeFont(1.5));
	pageName->setAlignment(Qt::AlignCenter);

	QSizePolicy expandToAl(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
	scanned->setSizePolicy(expandToAl);	//	buttons use all empty space
	scanner->setSizePolicy(expandToAl);
	receive->setSizePolicy(expandToAl);
	settings->setSizePolicy(expandToAl);
	setFocus();
	
#ifdef QT_VERSION5X
	QObject::connect(scanner, &QPushButton::clicked, this, &BranchRootWidget::scannerPressed);
	QObject::connect(scanned, &QPushButton::clicked, this, &BranchRootWidget::scannedPressed);
	QObject::connect(receive, &QPushButton::clicked, this, &BranchRootWidget::receivePressed);
	QObject::connect(settings, &QPushButton::clicked, this, &BranchRootWidget::settingsPressed);
	QObject::connect(backButton, &QPushButton::clicked, this, &BranchRootWidget::backRequire);
#else
	QObject::connect(scanner, SIGNAL(clicked()), this, SLOT(scannerPressed()));
	QObject::connect(scanned, SIGNAL(clicked()), this, SLOT(scannedPressed()));
	QObject::connect(receive, SIGNAL(clicked()), this, SLOT(receivePressed()));
	QObject::connect(settings, SIGNAL(clicked()), this, SLOT(settingsPressed()));
	QObject::connect(backButton, SIGNAL(clicked()), this, SLOT(backRequire()));
#endif
}

bool BranchRootWidget::giveSettings()
{
	if (currentlyOpened->giveSettings())
		return true;
	else
	{
		_hideAndDeleteCurrent( _allocateSettings());
	}
	return true;
}

bool BranchRootWidget::isExpectingControl(int contr)
{
	if (currentlyOpened == untouchable)
	{
		switch (contr)
		{
		case 0:
			scannerPressed();
			break;
		case 1:
			scannedPressed();
			break;
		case 2:
			receivePressed();
			break;
		case 3:
			backRequire();
			break;
		default:
			break;
		}
	}
	return true;
}

void BranchRootWidget::scannerPressed()
{
	_hideAndDeleteCurrent(_allocateScaning());
}
void BranchRootWidget::scannedPressed()
{
	_hideAndDeleteCurrent(_allocateStorage());
}

void BranchRootWidget::receivePressed()
{
	_hideAndDeleteCurrent(new SendingDataPickerWidget(currentMode, this));
#ifdef QT_VERSION5X
	QObject::connect(_upCO<SendingDataPickerWidget>(), &SendingDataPickerWidget::backRequired, this, &BranchRootWidget::backRequire);
#else
	QObject::connect(_upCO<SendingDataPickerWidget>(), SIGNAL(backRequired()), this, SLOT(backRequire()));
#endif
}

void BranchRootWidget::settingsPressed()
{
	_hideAndDeleteCurrent(_allocateSettings());
}

void BranchRootWidget::newBarcodeArrived(Entity e)
{
	AppData->postEntityInto(TableNames::Scanned, e, currentMode);
	AppBackup->pushOperation(OpType::SCAN, int(currentMode), e);
}


void BranchRootWidget::backRequire()
{
	if (untouchable->isHidden())
	{
		_hideAndDeleteCurrent(untouchable);
	}
	else
	{
		emit backRequired();
	}
}
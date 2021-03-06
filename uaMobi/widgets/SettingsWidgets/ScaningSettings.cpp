#include "ScaningSettings.h"
#include <QKeyEvent>
#include "widgets/utils/ElementsStyles.h"
#include "submodules/UNAQtCommons/barcodeHandling/BarcodeObserver.h"
#include "widgets/utils/GlobalAppSettings.h"
#include "submodules/UNAQtCommons/widgets/UtilityElements/MegaIconButton.h"
#include <QSpinBox>
#include <QFormLayout>


void Capturer::keyReleaseEvent(QKeyEvent* kev)
{
	emit keyCaptured(kev->key());
	setText(QString::number(kev->key()));
	return;
}

Capturer::Capturer(QWidget* parent)
	: QLineEdit(parent)
{

}

ScaningSettings::ScaningSettings(Modes cmode, QWidget* parent)
	: QWidget(parent), mainLayout(new QFormLayout(this)),
	prefixCapturer(new QSpinBox(this)), 
	suffixCapturer(new QSpinBox(this)),
	scanButtonCapturer(new Capturer(this)),
	additionInputElements(new MegaIconButton(this)),
	navigationElements(new MegaIconButton(this)),
	historyButton(new MegaIconButton(this)), currentMode(cmode)
{
	mainLayout->addRow(tr("Enter prefix code"), prefixCapturer);
	mainLayout->addRow(tr("Enter suffix code"), suffixCapturer);
	mainLayout->addRow(tr("Scan key setter"), scanButtonCapturer);
	mainLayout->addRow(tr("More input"), additionInputElements);
	mainLayout->addRow(tr("Navigation elements"), navigationElements);
	mainLayout->addRow(tr("Scan history"), historyButton);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(0);
	mainLayout->setRowWrapPolicy(QFormLayout::WrapAllRows);
	this->setFont(GENERAL_FONT);
	scanButtonCapturer->setText(QString::number(AppSettings->scanButtonCode));
	suffixCapturer->setValue(AppSettings->scanSuffix);
	prefixCapturer->setValue(AppSettings->scanPrefix);
	additionInputElements->setCheckable(true);
	additionInputElements->setChecked(AppSettings->additionalControlElements);
	additionInputElements->setStyleSheet(CHECKED_BUTTONS_STYLESHEET);
	additionInputElements->setIcon(QIcon(":/resources/toinput"));

	navigationElements->setCheckable(true);
	navigationElements->setChecked(AppSettings->navigationElements);
	navigationElements->setStyleSheet(CHECKED_BUTTONS_STYLESHEET);
	navigationElements->setIcon(QIcon(":/resources/forward"));

	historyButton->setCheckable(true);
	historyButton->setChecked(AppSettings->getModeDescription(cmode).isHistoryRequired());
	historyButton->setStyleSheet(CHECKED_BUTTONS_STYLESHEET);
	historyButton->setIcon(QIcon(":/resources/pen2"));


#ifdef QT_VERSION5X
	QObject::connect(scanButtonCapturer, &Capturer::keyCaptured, this, &ScaningSettings::scanKeyPressed);
#else
	QObject::connect(scanButtonCapturer, SIGNAL(keyCaptured(int)), this, SLOT(scanKeyPressed(int)));
#endif
}

void ScaningSettings::extractAndSave()
{
	AppSettings->additionalControlElements = additionInputElements->isChecked();
	AppSettings->navigationElements = navigationElements->isChecked();
	AppSettings->scanPrefix = prefixCapturer->value();
	AppSettings->scanSuffix = suffixCapturer->value();
	AppSettings->getModeDescription(currentMode).setHistoryRequire(historyButton->isChecked());
	BarcodeObs->resetCapture(AppSettings->scanPrefix, AppSettings->scanSuffix);
}

void ScaningSettings::scanKeyPressed(int keycode)
{
	AppSettings->scanButtonCode = keycode;
	BarcodeObs->resetCapture(AppSettings->scanPrefix, AppSettings->scanSuffix);
}


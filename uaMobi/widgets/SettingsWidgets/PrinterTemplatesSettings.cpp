#include "PrinterTemplatesSettings.h"
#include <qlistwidget.h>
#include <QBoxLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QScroller>
#include "submodules/UNAQtCommons/widgets/UtilityElements/MegaIconButton.h"
#include "widgets/utils/ElementsStyles.h"
#include <qapplication.h>
#include <QFile>
#include <qmessagebox.h>
#include "submodules/UNAQtCommons/widgets/UtilityElements/AndroidFileDialog.h"
#ifdef Q_OS_ANDROID
#include <qinputmethod.h>
#include <QScroller>
#endif
#include <QTextStream>
#include "widgets/utils/GlobalAppSettings.h"
#include <QFormLayout>
#include <algorithm>

PrinterTemplatesSettings::PrinterTemplatesSettings(QWidget* parent)
	: inframedWidget(parent), mainLayout(new QVBoxLayout(this)),
	tabs(new QTabWidget(this)),
	printerSettings(new QWidget(this)),
	printerLayout(new QFormLayout(this)),
	printerNameField(new QLineEdit(this)),
	bindingRedactor(new PrinterTemplatesListWidget(this)),
	backButton(new MegaIconButton(this))
{
	mainLayout->setSpacing(0);				//	Removed spacing for both layouts
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->addWidget(tabs);
	tabs->addTab(printerSettings, tr("Printer"));
	printerSettings->setLayout(printerLayout);
	printerLayout->addRow(tr("printer device name"), printerNameField);
	tabs->addTab(bindingRedactor, tr("Template"));
	mainLayout->addWidget(backButton);
	backButton->setText(tr("back"));
	backButton->setIcon(QIcon(":/res/back.png"));
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);
	printerNameField->setText(AppSettings->labelPrinterName);
	QObject::connect(backButton, &MegaIconButton::clicked, this, &PrinterTemplatesSettings::backPressed);
}

void PrinterTemplatesSettings::backPressed()
{
	AppSettings->labelPrinterName = printerNameField->text();
	PrinterTemplate currentOne = bindingRedactor->getCurrentTemplate();
	if (!currentOne.isNull())
	{
		AppSettings->labelPrinterTemplateText = currentOne->templateText;
		AppSettings->labelPrinterTemplateGUID = currentOne->GUID;
	}
	AppSettings->Save();
	emit backRequired();
}

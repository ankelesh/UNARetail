#include "PrinterTemplatesSettings.h"
#include <qlistwidget.h>
#include <QBoxLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QScroller>
#include "widgets/utils/MegaIconButton.h"
#include "widgets/utils/ElementsStyles.h"
#include <qapplication.h>
#include <QFile>
#include <qmessagebox.h>
#include "widgets/UtilityElements/AndroidFileDialog.h"
#ifdef Q_OS_ANDROID
#include <qinputmethod.h>
#include <QScroller>
#endif
#include <QTextStream>
#include "widgets/utils/GlobalAppSettings.h"
#include <QFormLayout>
void PrinterTemplateRedactor::_fill_editor(QString fname)
{
	QFile edited_file(fname);
	edited_file.open(QIODevice::ReadOnly);
	templateFileStatus = true;
	currentFilePath = fname;
	QTextStream in(&edited_file);
	bindingTemplateField->setText(in.readAll());
	edited_file.close();
}

void PrinterTemplateRedactor::_init_defaults()
{
	if (templatePath != Q_NULLPTR)
	{
		bindingPathField->setText(*templatePath);
		if (QFile::exists(*templatePath))
			_fill_editor(*templatePath);
	}
    else
    {
        bindingPathField->setText(rootFilePath);
    }
	if (!templateFileStatus)
	{
		if (templateText != Q_NULLPTR)
			bindingTemplateField->setText(*templateText);
	}
}

PrinterTemplateRedactor::PrinterTemplateRedactor(QString* template_text, QString* template_path, QWidget* parent)
	: QWidget(parent), bindingLayout(new QVBoxLayout(this)),
	pathLayout(new QHBoxLayout(this)), bindingPathField(new QLineEdit(this)),
	fileDialogButton(new MegaIconButton(QIcon(":/res/add.png"),0.9, QString(), this)), bindingTemplateField(new QTextEdit(this)),
	bindingInfo(new QListWidget(this)), templateFileStatus(false), currentFilePath(),
	templateText(template_text), templatePath(template_path)
{
	bindingLayout->addLayout(pathLayout);
	pathLayout->addWidget(bindingPathField);
	pathLayout->addWidget(fileDialogButton);
	bindingLayout->addWidget(bindingTemplateField);
	bindingLayout->addWidget(bindingInfo);
    fileDialogButton->setMaximumHeight(calculateAdaptiveButtonHeight(0.03));
    bindingTemplateField->setMinimumHeight(calculateAdaptiveButtonHeight(0.2));
    bindingTemplateField->setMaximumHeight(calculateAdaptiveButtonHeight(0.4));
    bindingInfo->setMaximumHeight(calculateAdaptiveButtonHeight(0.1));
	_init_defaults();
#ifdef Q_OS_ANDROID
	QScroller::grabGesture(bindingInfo, QScroller::LeftMouseButtonGesture);
	bindingInfo->setVerticalScrollMode(QListWidget::ScrollPerPixel);
#endif
#ifdef QT_VERSION5X
	QObject::connect(bindingPathField, &QLineEdit::returnPressed, this, &PrinterTemplateRedactor::openFile);
#ifdef Q_OS_ANDROID
	QObject::connect(bindingPathField, &QLineEdit::returnPressed, qApp->inputMethod(), &QInputMethod::hide);
#endif
	QObject::connect(fileDialogButton, &MegaIconButton::clicked, this, &PrinterTemplateRedactor::openByDialog);
#else
#endif
	
}

void PrinterTemplateRedactor::saveData()
{
	if (templateFileStatus)
	{
		QFile out(currentFilePath);
		out.open(QIODevice::WriteOnly);
		QTextStream stream(&out);
		stream << bindingTemplateField->toPlainText();
		stream.flush();
		out.close();
		if (templatePath != Q_NULLPTR)
		{
			(*templatePath) = currentFilePath;
		}
	}
	if (!bindingTemplateField->toPlainText().isEmpty())
	{
		if (templateText != Q_NULLPTR)
		{
			(*templateText) = bindingTemplateField->toPlainText();
		}
	}
	
}

void PrinterTemplateRedactor::openFile()
{
	if (QFile::exists(bindingPathField->text()))
	{
		_fill_editor(bindingPathField->text());
	}
	else
	{
		bindingPathField->clear();
		QMessageBox::critical(this, tr("File not exists!"), tr("File not exists!"));
	}
}

void PrinterTemplateRedactor::openByDialog()
{		
	QString path;
	if (templatePath != Q_NULLPTR)
        path = AndroidFileDialog::getFileName(bindingPathField->text());
	else
        path = AndroidFileDialog::getFileName(bindingPathField->text());
	if (!path.isEmpty())
	{
		_fill_editor(bindingPathField->text());
	}
}

PrinterTemplatesSettings::PrinterTemplatesSettings(QWidget* parent)
	: inframedWidget(parent), mainLayout(new QVBoxLayout(this)),
	tabs(new QTabWidget(this)),
	printerSettings(new QWidget(this)),
	printerLayout(new QFormLayout(this)),
	printerNameField(new QLineEdit(this)),
	bindingRedactor(new PrinterTemplateRedactor(
		&AppSettings->labelPrinterTemplateText, 
		&AppSettings->labelPrinterTemplatePath, this)),
	backButton(new MegaIconButton(this))
{
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
	bindingRedactor->saveData();
	AppSettings->labelPrinterName = printerNameField->text();

	emit backRequired();
}

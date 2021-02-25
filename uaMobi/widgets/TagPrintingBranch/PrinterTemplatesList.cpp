#include "PrinterTemplatesList.h"
#include "dataproviders/sqldataprovider.h"
#include "submodules/UNAQtCommons/widgets/UtilityElements/ExtendedDialogs.h"
#include "submodules/UNAQtCommons/widgets/UtilityElements/AndroidFileDialog.h"
#include "widgets/utils/GlobalAppSettings.h"
#ifdef Q_OS_ANDROID
#include "submodules/UNAQtCommons/wrappers/Platform/Android/androidservicewrapper.h"
#endif
#include <QListView>
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
#include <QSpinBox>
#ifdef Q_OS_ANDROID
#include <qinputmethod.h>
#include <QScroller>
#endif
#include <QTextStream>
#include <algorithm>

QString _extractFileName(QString fname)
{
	QString buffer;
	buffer.reserve(20);
	QString::iterator dot_pos = fname.end();
	QString::iterator slash_pos = fname.end();
	for (QString::iterator ch = fname.end() - 1; ch != fname.begin(); --ch)
	{
		switch (ch->toLatin1())
		{
		case '\\':
		case '/':
			slash_pos = ch + 1;
			break;
		case '.':
			dot_pos = ch;
			break;
		}
		if (slash_pos != fname.end())
			break;
	}
	while (slash_pos != dot_pos)
	{
		buffer.push_back(*slash_pos);
		++slash_pos;
	}
	return buffer;
}


void PrinterTemplateRedactor::_fill_editor(QString fname)
{
	QFile edited_file(fname);
	edited_file.open(QIODevice::ReadOnly);
	QTextStream in(&edited_file);
	bindingTemplateField->setText(in.readAll());
	edited_file.close();
	templateNameField->setText(_extractFileName(fname));
	widthField->setValue(65);
	heightField->setValue(39);
}

PrinterTemplateRedactor::PrinterTemplateRedactor(QWidget* parent)
	: inframedWidget(parent), bindingLayout(new QVBoxLayout(this)),
	templateNameField(new QLineEdit(this)),
	bindingTemplateField(new QTextEdit(this)),
	sizeLayout(new QHBoxLayout(this)),
	widthInfo(new QLabel(tr("width"), this)),
	widthField(new QSpinBox(this)),
	heightInfo(new QLabel(tr("height"), this)),
	heightField(new QSpinBox(this)),
	bindingInfo(new QListWidget(this)),
	buttonsLayout(new QHBoxLayout(this)),
	cancelButton(new MegaIconButton(this)),
	okButton(new MegaIconButton(this)),
	redactedTemplate(),
	isNewTemplate(false)
{
	bindingLayout->setSpacing(0);				//	Removed spacing for both layouts
	bindingLayout->setContentsMargins(0, 0, 0, 0);
	bindingLayout->addWidget(templateNameField);
	bindingLayout->addWidget(bindingTemplateField);
	bindingLayout->addLayout(sizeLayout);
	sizeLayout->addWidget(widthInfo);
	sizeLayout->addWidget(widthField);
	sizeLayout->addWidget(heightInfo);
	sizeLayout->addWidget(heightField);
	bindingLayout->addWidget(bindingInfo);
	bindingLayout->addLayout(buttonsLayout);
	buttonsLayout->addWidget(cancelButton);
	buttonsLayout->addWidget(okButton);
	bindingTemplateField->setMinimumHeight(calculateAdaptiveHeight(0.2));
	bindingTemplateField->setMaximumHeight(calculateAdaptiveHeight(0.4));
	bindingInfo->setMaximumHeight(calculateAdaptiveHeight(0.1));
	cancelButton->setText(tr("cancel"));
	cancelButton->setStyleSheet(DELETE_BUTTONS_STYLESHEET);
	cancelButton->setIcon(QIcon(":/resources/nodata"));
	okButton->setText(tr("OK"));
	okButton->setIcon(QIcon(":/resources/ok"));
	okButton->setStyleSheet(OK_BUTTONS_STYLESHEET);
#ifdef Q_OS_ANDROID
	QScroller::grabGesture(bindingInfo, QScroller::LeftMouseButtonGesture);
	bindingInfo->setVerticalScrollMode(QListWidget::ScrollPerPixel);
	QScroller::grabGesture(bindingTemplateField, QScroller::LeftMouseButtonGesture);
#endif
#ifdef QT_VERSION5X
	QObject::connect(cancelButton, &MegaIconButton::clicked, this, &PrinterTemplateRedactor::backRequired);
	QObject::connect(okButton, &MegaIconButton::clicked, this, &PrinterTemplateRedactor::onOkButtonPressed);
#endif

}

void PrinterTemplateRedactor::redactExisting(PrinterTemplate ptemplate)
{
	redactedTemplate = ptemplate;
	isNewTemplate = false;
	bindingTemplateField->setText(ptemplate->templateText);
	templateNameField->setText(ptemplate->templateName);
	heightField->setValue(ptemplate->height);
	widthField->setValue(ptemplate->width);
}

void PrinterTemplateRedactor::openNewTemplate(QString filepath)
{
	if (QFile::exists(filepath))
	{
		_fill_editor(filepath);
		isNewTemplate = true;
	}
	else
	{
		templateNameField->setText("newTemplate");
		bindingTemplateField->setText("");
		widthField->setValue(65);
		heightField->setValue(39);
		isNewTemplate = true;
	}
}

void PrinterTemplateRedactor::onOkButtonPressed()
{
	if (isNewTemplate)
	{
		PrinterTemplate newObj(new PrinterTemplateEntity(
			templateNameField->text(),
			bindingTemplateField->toPlainText(),
			widthField->value(),
			heightField->value()
		));
		if (newObj->isValid())
			emit newAdded(newObj);
	}
	else
	{
		PrinterTemplate newObj(new PrinterTemplateEntity(
			templateNameField->text(),
			bindingTemplateField->toPlainText(),
			widthField->value(),
			heightField->value()
		));
		if (newObj->isValid())
		{
			emit redactingFinished(redactedTemplate, newObj);
		}
	}
}


PrinterTemplatesListWidget::PrinterTemplatesListWidget(QWidget* parent)
	:	inframedWidget(parent), abstractDynamicNode(new inframedWidget(this), new QVBoxLayout(this)),
		innerLayout(new QVBoxLayout(untouchable)),
	topLayout(new QHBoxLayout(untouchable)), explanationLabel(new QLabel(untouchable)),
	newButton(new MegaIconButton(untouchable)), mainView(new QListView(untouchable)),
	buttonsLayout(new QHBoxLayout(untouchable)), deleteButton(new MegaIconButton(untouchable)),
	shareButton(new MegaIconButton(untouchable)), editButton(new MegaIconButton(untouchable)),
	model(new DataEntityListModel(this))
{
	innerLayout->setSpacing(0);				//	Removed spacing for both layouts
	innerLayout->setContentsMargins(0, 0, 0, 0);
	untouchable->setLayout(innerLayout);
	innerLayout->addLayout(topLayout);
	innerLayout->addWidget(mainView);
	innerLayout->addLayout(buttonsLayout);
	topLayout->addWidget(explanationLabel);
	topLayout->addWidget(newButton);
	buttonsLayout->addWidget(deleteButton);
	buttonsLayout->addWidget(shareButton);
	buttonsLayout->addWidget(editButton);
	explanationLabel->setText("select template or add new");
	newButton->setIcon(QIcon(":/resources/plus"));
	newButton->setText(tr("new"));
	newButton->setMaximumHeight(calculateAdaptiveHeight(0.9));;
	newButton->setMaximumWidth(calculateAdaptiveWidth(0.15));
	deleteButton->setIcon(QIcon(":/resources/nodata"));
	deleteButton->setText(tr("delete"));
	shareButton->setIcon(QIcon(":/res/share.png"));
	shareButton->setText(tr("share"));
	editButton->setIcon(QIcon(":/resources/pencil"));
	editButton->setText(tr("edit"));

	model->insertData(downcastEntityList<PrinterTemplateEntity>(AppData->getPrinterTemplates()));
	mainView->setModel(model);
	mainView->setSelectionMode(QListView::SingleSelection);
	mainView->setCurrentIndex(model->findByGuid(AppSettings->labelPrinterTemplateGUID));
	QObject::connect(newButton, &MegaIconButton::clicked, this, &PrinterTemplatesListWidget::onNewTemplatePress);
	QObject::connect(deleteButton, &MegaIconButton::clicked, this, &PrinterTemplatesListWidget::onDeleteButtonPress);
	QObject::connect(shareButton, &MegaIconButton::clicked, this, &PrinterTemplatesListWidget::onShareButtonPress);
	QObject::connect(editButton, &MegaIconButton::clicked, this, &PrinterTemplatesListWidget::onEditButtonPress);
}

PrinterTemplate PrinterTemplatesListWidget::getCurrentTemplate()
{
	QModelIndex currentIndex = mainView->currentIndex();
	if (currentIndex.isValid())
	{
		return upcastEntity<PrinterTemplateEntity>(model->directAccessByFlatIndex(currentIndex.row()));
	}
	else
		return PrinterTemplate();
}

void PrinterTemplatesListWidget::onNewTemplatePress()
{
	editor = new PrinterTemplateRedactor(this);
	QObject::connect(editor, &PrinterTemplateRedactor::backRequired, this, &PrinterTemplatesListWidget::hideCurrent);
	QObject::connect(editor, &PrinterTemplateRedactor::redactingFinished, this, &PrinterTemplatesListWidget::onEditDone);
	QObject::connect(editor, &PrinterTemplateRedactor::newAdded, this, &PrinterTemplatesListWidget::onNewOpenDone);
	editor->openNewTemplate(AndroidFileDialog::getFileName(rootFilePath));
	_hideAndDeleteCurrent(editor);
}

void PrinterTemplatesListWidget::onDeleteButtonPress()
{
	QModelIndex currentIndex = mainView->currentIndex();
	if (currentIndex.isValid())
	{
		AppData->deletePrinterTemplate(upcastEntity<PrinterTemplateEntity>(model->directAccessByFlatIndex(currentIndex.row())));
		model->removeDataEntity(currentIndex);
	}
}

void PrinterTemplatesListWidget::onShareButtonPress()
{
	QModelIndex currentIndex = mainView->currentIndex();
	if (currentIndex.isValid())
	{
#ifdef Q_OS_ANDROID
         AndroidServiceWrapper::instance().sendMessageIntent(
			 model->directAccessByFlatIndex(
				 currentIndex.row())
			 ->getWriteable(Roles::PrinterTemplate::templateText));
#endif
	}
}

void PrinterTemplatesListWidget::onEditButtonPress()
{
	editor = new PrinterTemplateRedactor(this);
	QObject::connect(editor, &PrinterTemplateRedactor::backRequired, this, &PrinterTemplatesListWidget::hideCurrent);
	QObject::connect(editor, &PrinterTemplateRedactor::redactingFinished, this, &PrinterTemplatesListWidget::onEditDone);
	QObject::connect(editor, &PrinterTemplateRedactor::newAdded, this, &PrinterTemplatesListWidget::onNewOpenDone);
	QModelIndex currentIndex = mainView->currentIndex();
	if (currentIndex.isValid())
	{
		editor->redactExisting(upcastEntity<PrinterTemplateEntity>(model->directAccessByFlatIndex(currentIndex.row())));
	}
	_hideAndDeleteCurrent(editor);
}

void PrinterTemplatesListWidget::onEditDone(PrinterTemplate old, PrinterTemplate newOne)
{
	AppData->deletePrinterTemplate(old);
	AppData->addPrinterTemplate(newOne);
	model->removeDataEntity(old);
	model->appendDataEntity(newOne);
	hideCurrent();
}

void PrinterTemplatesListWidget::onNewOpenDone(PrinterTemplate newOne)
{
	AppData->addPrinterTemplate(newOne);
	model->appendDataEntity(newOne);
	hideCurrent();
}

void PrinterTemplatesListWidget::hideCurrent()
{
	_hideAndDeleteCurrent(untouchable);
}


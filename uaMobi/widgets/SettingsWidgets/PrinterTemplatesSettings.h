#pragma once
#include "widgets/parents/inframedWidget.h"



class QTabWidget;
class MegaIconButton;
class QFormLayout;
class QLineEdit;
class QTextEdit;
class QVBoxLayout;
class QHBoxLayout;
class QLabel;
class QListWidget;

class PrinterTemplateRedactor : public QWidget
{
	Q_OBJECT
protected:
	QVBoxLayout* bindingLayout;
	QHBoxLayout* pathLayout;
	QLineEdit* bindingPathField;
	MegaIconButton* fileDialogButton;
	QTextEdit* bindingTemplateField;
	QListWidget* bindingInfo;
	bool templateFileStatus;
	QString currentFilePath;

	QString * templateText;
	QString * templatePath;

	void _fill_editor(QString fname);
	void _init_defaults();
public:
	PrinterTemplateRedactor(QString * template_text, QString * template_path, QWidget* parent);
	void saveData();
protected slots:
	void openFile();
	void openByDialog();
};




class PrinterTemplatesSettings : public inframedWidget
{
	Q_OBJECT
protected:
	QVBoxLayout* mainLayout;
	QTabWidget* tabs;

	QWidget* printerSettings;
	QFormLayout* printerLayout;
	QLineEdit* printerNameField;
	
	PrinterTemplateRedactor* bindingRedactor;
	MegaIconButton* backButton;
public:
	PrinterTemplatesSettings(QWidget* parent);
protected slots:
	void backPressed();
};
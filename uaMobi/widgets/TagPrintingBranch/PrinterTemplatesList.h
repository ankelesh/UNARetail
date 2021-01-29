#pragma once
#include "widgets/parents/inframedWidget.h"
#include "Datacore/DataEntities.h"
#include "widgets/parents/abstractNodeInterface.h"

class QTabWidget;
class MegaIconButton;
class QFormLayout;
class QLineEdit;
class QTextEdit;
class QVBoxLayout;
class QHBoxLayout;
class QLabel;
class QListWidget;
class QListView;
class QSpinBox;
class PrinterTemplateRedactor : public inframedWidget
{
	Q_OBJECT
protected:
	QVBoxLayout* bindingLayout;
	QLineEdit* templateNameField;
	QTextEdit* bindingTemplateField;
	QHBoxLayout* sizeLayout;
	QLabel* widthInfo;
	QSpinBox* widthField;
	QLabel* heightInfo;
	QSpinBox* heightField;
	QListWidget* bindingInfo;
	QHBoxLayout* buttonsLayout;
	MegaIconButton* cancelButton;
	MegaIconButton* okButton;

	PrinterTemplate redactedTemplate;
	bool isNewTemplate;

	void _fill_editor(QString fname);
public:
	PrinterTemplateRedactor(QWidget* parent);
	void redactExisting(PrinterTemplate ptemplate);
	void openNewTemplate(QString filepath);
protected slots:
	void onOkButtonPressed();
signals:
	void newAdded(PrinterTemplate t);
	void redactingFinished(PrinterTemplate old, PrinterTemplate redacted);
};




class PrinterTemplatesListWidget : public inframedWidget, public abstractDynamicNode
{
	Q_OBJECT
protected:
	QVBoxLayout* innerLayout;
	QHBoxLayout* topLayout;
	QLabel* explanationLabel;
	MegaIconButton* newButton;
	QListView* mainView;
	QHBoxLayout* buttonsLayout;
	MegaIconButton* deleteButton;
	MegaIconButton* shareButton;
	MegaIconButton* editButton;

	PrinterTemplateRedactor* editor;

	DataEntityListModel* model;
public:
	PrinterTemplatesListWidget(QWidget* parent);
	PrinterTemplate getCurrentTemplate();
protected slots:
	void onNewTemplatePress();
	void onDeleteButtonPress();
	void onShareButtonPress();
	void onEditButtonPress();
	void onEditDone(PrinterTemplate, PrinterTemplate);
	void onNewOpenDone(PrinterTemplate);
	void hideCurrent();
};
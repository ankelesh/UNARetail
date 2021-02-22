#pragma once
#include "submodules/UNAQtCommons/widgets/parents/inframedWidget.h"
#include "Datacore/DataEntities.h"
#include "widgets/TagPrintingBranch/PrinterTemplatesList.h"

class QTabWidget;
class MegaIconButton;
class QFormLayout;
class QLineEdit;
class QTextEdit;
class QVBoxLayout;
class QHBoxLayout;
class QLabel;
class QListWidget;




class PrinterTemplatesSettings : public inframedWidget
{
	Q_OBJECT
protected:
	QVBoxLayout* mainLayout;
	QTabWidget* tabs;

	QWidget* printerSettings;
	QFormLayout* printerLayout;
	QLineEdit* printerNameField;
	
	PrinterTemplatesListWidget* bindingRedactor;
	MegaIconButton* backButton;
public:
	PrinterTemplatesSettings(QWidget* parent);
protected slots:
	void backPressed();
};
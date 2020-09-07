#pragma once
#include <QWidget>
#include "dataproviders/ModesDescriptions.h"

class QFormLayout;
class QLabel;
class QComboBox;
class MegaIconButton;
class QLineEdit;
class QuickSendSettings : public QWidget
{
	Q_OBJECT
protected:
	QFormLayout* mainLayout;
	QLabel* aboutWidget;
	QComboBox* protocolPicker;
	QComboBox* sendingFormatPicker;
	MegaIconButton* simpleSendingButton;
	MegaIconButton* attachLastDoc;
	MegaIconButton* cleanLastDoc;
	QLineEdit* docField;
	Modes currentMode;
public:
	QuickSendSettings(Modes mode, QWidget * parent);
	void extractAndSave();
	void attachClicked();
};
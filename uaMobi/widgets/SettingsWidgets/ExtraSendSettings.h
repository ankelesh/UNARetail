#pragma once
#include <QWidget>
#include "dataproviders/ModesDescriptions.h"

class QFormLayout;
class QComboBox;
class QSpinBox;
class ExtraSendSettings : public QWidget
{
	Q_OBJECT
protected:
	QFormLayout* mainLayout;
	QComboBox* sysfeedPicker;
	QSpinBox* customSysfeedPicker;
	Modes currentMode;
	void _initSysfeeds();
public:
	ExtraSendSettings(Modes cmode,QWidget* parent);
	void extractAndSave();

protected slots:
	void itemSelected(int index);
};
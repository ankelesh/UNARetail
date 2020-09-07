#pragma once
#include <QWidget>

class QFormLayout;
class QSpinBox;
class QLabel;
class ViewSettings : public QWidget
{
	Q_OBJECT
protected:
	QFormLayout* mainLayout;
	QLabel* example;
	QSpinBox* fontMin;
	QSpinBox* fontMax;
	QSpinBox* fontPercent;

public:
	ViewSettings(QWidget* parent);
	void extractAndSave();
	void retranslate();
protected slots:
	void refreshExample();
};
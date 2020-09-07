#pragma once
#include <QWidget>
#include <QVector>
#include <QIcon>
class QFormLayout;
class QLineEdit;
class MegaIconButton;
class QComboBox;
class QLabel;
class GeneralSettings : public QWidget
{
	Q_OBJECT
protected:
	QFormLayout* mainLayout;
	QLineEdit* localPath;
	MegaIconButton* langButton;
	QLabel* versionControl;
	QComboBox* encodingPicker;
	int currlang;

	QVector<QIcon> langicons;
public:
	GeneralSettings(QWidget* parent);
	void extractAndSave();

protected slots:
	void langChangePressed();
	void encodingPicked(const QString &);
signals:
	void retranslated();
};
#pragma once
#include <QWidget>
#include "dataproviders/ModesDescriptions.h"

class QVBoxLayout;
class MegaIconButton;
class DatabaseSettings : public QWidget
{
	Q_OBJECT
protected:
	QVBoxLayout* mainLayout;
	MegaIconButton* clearScanned;
	MegaIconButton* wipeDatabase;
	MegaIconButton* clearLogs;
	MegaIconButton* searchOnScan;

	Modes currentMode;
public:
	DatabaseSettings(Modes mode, QWidget* parent);
	void extractAndSave();
protected slots:
	void clearScannedConfirmed();
	void wipeDatabaseConfirmed();
	void clearLogsConfirmed();
};
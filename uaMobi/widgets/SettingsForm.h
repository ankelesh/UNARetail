#pragma once
#include "widgets/parents/inframedWidget.h"
/*
*/
class QVBoxLayout;
class GeneralSettings;
class SearchDatabaseSettings;
class ViewSettings;
class DatabaseLoadSettings;
class MegaIconButton;
class QTabWidget;
class SettingsForm : public inframedWidget
{
	Q_OBJECT
private:
	QVBoxLayout* mainLayout;
	QTabWidget* innerWidget;
	GeneralSettings* generalSettings;
	SearchDatabaseSettings* databaseSettings;
	ViewSettings* viewSettings;
	DatabaseLoadSettings* dataloadSettings;
	MegaIconButton* backButton;
	
public:
	SettingsForm( QWidget* parent);

public slots:
	void retranslation();
	void saveAndExit();
	void toggleOverlay(bool);
signals:
	void retranslated();
	void fontsChanged();
};

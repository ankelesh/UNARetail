#include "SettingsForm.h"

#ifdef DEBUG
#include "debugtrace.h"
#endif
#include <QMessageBox>
#include "widgets/utils/ElementsStyles.h"
#include <qboxlayout.h>
#include "externalCommunication/tohttp.h"
#include "widgets/utils/GlobalAppSettings.h"
#include "widgets/utils/EventsAndFilters.h"
#include "widgets/utils/MegaIconButton.h"
#include "widgets/SettingsWidgets/GeneralSettings.h"
#include "widgets/SettingsWidgets/SearchDatabaseSettings.h"
#include "widgets/SettingsWidgets/ViewSettings.h"
#include "widgets/SettingsWidgets/DatabaseLoadSettings.h"

#include <qtabwidget.h>


SettingsForm::SettingsForm( QWidget* parent)
	: inframedWidget(parent), mainLayout(new QVBoxLayout(this)),
	innerWidget(new QTabWidget(this)),
	generalSettings(new GeneralSettings(this)),
	databaseSettings(new SearchDatabaseSettings(this)),
	viewSettings(new ViewSettings(this)),
	dataloadSettings(new DatabaseLoadSettings(this)),
	backButton(new MegaIconButton(this))
{
	mainLayout->addWidget(innerWidget);
	mainLayout->addWidget(backButton);
	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	setFont(AppGenFont);
	innerWidget->setMinimumHeight(calculateAdaptiveButtonHeight(0.8));
	innerWidget->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
	innerWidget->setLayout(new QVBoxLayout(innerWidget));
	
	innerWidget->addTab(generalSettings, tr("General"));
	innerWidget->addTab(databaseSettings, tr("Database"));
	innerWidget->addTab(viewSettings, tr("View"));
	innerWidget->addTab(dataloadSettings, tr("Load"));
	backButton->setText(tr("back"));
	backButton->setIcon(QIcon(":/res/back.png"));
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);
#ifdef QT_VERSION5X
	QObject::connect(backButton, &MegaIconButton::clicked, this, &SettingsForm::saveAndExit);
	QObject::connect(generalSettings, &GeneralSettings::retranslated, this, &SettingsForm::retranslation);
	QObject::connect(databaseSettings, &SearchDatabaseSettings::innerBranchSwitched, this, &SettingsForm::toggleOverlay);
#else
	QObject::connect(backButton, SIGNAL(clicked()), this, SLOT(saveAndExit()));
	QObject::connect(generalSettings, SIGNAL(retranslated()), this, SLOT(retranslation()));
#endif
}

void SettingsForm::retranslation()
{
	databaseSettings->retranslate();
	innerWidget->setTabText(0, tr("General"));
	innerWidget->setTabText(1, tr("Database"));
	innerWidget->setTabText(2, tr("View"));
	innerWidget->setTabText(3, tr("Load"));
	backButton->setText(tr("back"));
	viewSettings->retranslate();
	databaseSettings->retranslate();
	dataloadSettings->retranslate();
	emit retranslated();
}

void SettingsForm::saveAndExit()
{
	generalSettings->extractAndSave();
	databaseSettings->extractAndSave();
	viewSettings->extractAndSave();
	dataloadSettings->extractAndSave();
	AppSettings->Save();
	emit backRequired();
	emit fontsChanged();
}

void SettingsForm::toggleOverlay(bool require)
{
	if (require)
	{
		backButton->hide();
	}
	else
	{
		backButton->show();
	}
}









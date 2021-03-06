#include "SpecialActionsSettings.h"
#include "widgets/SettingsWidgets/EmailSettings.h"
#include "widgets/SettingsWidgets/PrinterSettings.h"
#include <QTabWidget>
#include <QBoxLayout>
#include "submodules/UNAQtCommons/widgets/UtilityElements/MegaIconButton.h"
#include "widgets/utils/GlobalAppSettings.h"
#include "widgets/utils/ElementsStyles.h"
#include "widgets/SettingsWidgets/PrinterConnectionSettings.h"
SpecialActionsSettings::SpecialActionsSettings(QWidget *parent) : inframedWidget(parent),
    mainLayout(new QVBoxLayout(this)),
    buttonLayout(new QHBoxLayout(this)),
    mainSettings(new QTabWidget(this)),
    okButton(new MegaIconButton(this)),
    backButton(new MegaIconButton(this)),
    pSettings(new PrinterSettings(this)),
    eSettings(new EmailSettings(this)),
    pcSettings(new PrinterConnectionSettings(this))
{
    mainLayout->addWidget(mainSettings);
    mainLayout->addLayout(buttonLayout);
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(backButton);
    mainSettings->addTab(pcSettings, tr("Connect"));
    mainSettings->addTab(pSettings, tr("Printer"));
    mainSettings->addTab(eSettings, tr("Email"));
    
    okButton->setText(tr("Ok"));
    okButton->setIcon(QIcon(":/resources/submit"));
    okButton->setStyleSheet(COMMIT_BUTTONS_STYLESHEET);

    backButton->setText(tr("back"));
    backButton->setIcon(QIcon(":/resources/back"));
    backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);
    QObject::connect(okButton, &MegaIconButton::clicked, this, &SpecialActionsSettings::saveAndExit);
    QObject::connect(backButton, &MegaIconButton::clicked, this, &SpecialActionsSettings::backRequired);

}

void SpecialActionsSettings::saveAndExit()
{
    pSettings->extractAndSave();
    eSettings->extractAndSave();
    AppSettings->save();
    emit backRequired();
}

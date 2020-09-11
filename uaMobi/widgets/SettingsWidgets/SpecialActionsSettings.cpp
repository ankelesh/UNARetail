#include "SpecialActionsSettings.h"
#include "widgets/SettingsWidgets/EmailSettings.h"
#include "widgets/SettingsWidgets/PrinterSettings.h"
#include <QTabWidget>
#include <QBoxLayout>
#include "widgets/utils/MegaIconButton.h"
SpecialActionsSettings::SpecialActionsSettings(QWidget *parent) : inframedWidget(parent),
    mainLayout(new QVBoxLayout(this)),
    buttonLayout(new QHBoxLayout(this)),
    mainSettings(new QTabWidget(this)),
    okButton(new MegaIconButton(this)),
    backButton(new MegaIconButton(this)),
    pSettings(new PrinterSettings(this)),
    eSettings(new EmailSettings(this))
{
    mainLayout->addWidget(mainSettings);
    mainLayout->addLayout(buttonLayout);
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(backButton);
    mainSettings->addTab(pSettings, tr("Printer"));
    mainSettings->addTab(eSettings, tr("Email"));

    QObject::connect(okButton, &MegaIconButton::clicked, this, &SpecialActionsSettings::saveAndExit);
    QObject::connect(backButton, &MegaIconButton::clicked, this, &SpecialActionsSettings::backRequired);

}

void SpecialActionsSettings::saveAndExit()
{
    pSettings->extractAndSave();
    eSettings->extractAndSave();
    emit backRequired();
}

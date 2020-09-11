#pragma once
#ifndef SPECIALACTIONSSETTINGS_H
#define SPECIALACTIONSSETTINGS_H
#include "widgets/parents/inframedWidget.h"


class PrinterSettings;
class EmailSettings;
class MegaIconButton;
class QVBoxLayout;
class QHBoxLayout;
class QTabWidget;
class SpecialActionsSettings : public inframedWidget
{
    Q_OBJECT
    QVBoxLayout* mainLayout;
    QHBoxLayout* buttonLayout;
    QTabWidget* mainSettings;
    MegaIconButton* okButton;
    MegaIconButton* backButton;
    PrinterSettings* pSettings;
    EmailSettings* eSettings;
public:
    explicit SpecialActionsSettings(QWidget *parent = Q_NULLPTR);
protected slots:
    void saveAndExit();
};

#endif // SPECIALACTIONSSETTINGS_H

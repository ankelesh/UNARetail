#pragma once
#ifndef EMAILSETTINGS_H
#define EMAILSETTINGS_H

#include <QWidget>

class QFormLayout;
class QLabel;
class QComboBox;
class MegaIconButton;
class QLineEdit;
class EmailSettings : public QWidget
{
    Q_OBJECT
protected:
    QFormLayout * mainLayout;
    MegaIconButton* enableEmailingButton;
    MegaIconButton* enableMessageButton;
    QLineEdit * targetAdresses;

public:
    explicit EmailSettings(QWidget *parent = nullptr);
    void extractAndSave();
};

#endif // EMAILSETTINGS_H

#pragma once
#ifndef PRINTERSETTINGS_H
#define PRINTERSETTINGS_H

#include <QWidget>


class QVBoxLayout;
class inframedWidget;
class QFormLayout;
class QLabel;
class MegaIconButton;
class QLineEdit;
class QProgressBar;
class toHttp;
class QListWidget;
class QTimer;
class QListWidgetItem;
class QSpinBox;
class PrinterSettings : public QWidget
{
    Q_OBJECT
private:
    QFormLayout * mainLayout;
    QLineEdit* printerIp;
    QSpinBox* printerPort;
    QLineEdit* userName;
    QLineEdit* printerPassword;
    QSpinBox * operatorNumber;
    QLineEdit* operatorPassword;
    MegaIconButton* toFile;
    QLineEdit* filePath;
public:
    PrinterSettings(QWidget* parent);
    void extractAndSave();
};

#endif // PRINTERSETTINGS_H

#pragma once

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
class PrinterConnectionSettings : public QWidget
{
    Q_OBJECT
private:
    QFormLayout* mainLayout;
    QLineEdit* printerIp;
    QSpinBox* printerPort;
    QLineEdit* printerPassword;
    QLineEdit* printerServerAddress;
    MegaIconButton* comInsteadTcp;
public:
    PrinterConnectionSettings(QWidget* parent);
    void extractAndSave();
};

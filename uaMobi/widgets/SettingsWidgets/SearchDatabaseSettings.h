#pragma once
#include <QWidget>
#include "submodules/UNAQtCommons/widgets/parents/abstractNodeInterface.h"
#include <QString>
#include <QStringList>

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
class SearchDatabaseSettings : public QWidget,  abstractNode
{
	Q_OBJECT
protected:
	QVBoxLayout* innerLayout;
	inframedWidget* innerWidget;
	QFormLayout* mainLayout;
	QLabel* storedBarcodes;
	QLineEdit* httpDownloadUrl;
	QHBoxLayout* placeLayout;
	QLabel* placeInfo;
	MegaIconButton* selectPlaceButton;
	MegaIconButton* deletePlaceButton;
	MegaIconButton* downloadNow;
	QProgressBar* downloadProgress;
	toHttp* downloadcenter;
	QTimer* timeoutTimer;

	inframedWidget* placeSelectionWidget;
	QVBoxLayout* placeSelectionLayout;
	QListWidget* placeSelectionList;
	MegaIconButton* backButton;

	QStringList placeCodes;
	QStringList placeNames;
	QString currentPlace;
	QString currentPlaceName;

	int nextPage;

	QString _getCurrentPlace();
public:
	SearchDatabaseSettings(QWidget* parent);
	void extractAndSave();
	void retranslate();
	void refreshStoredCounter();
protected slots:
	void downloadDatabase();
	void downloadTimeout();
	void downloadProcess(int);
	void getPlaceList();
	void placeListReceived(QStringList names, QStringList codes);
	void placeListTimeout();
	void placeSelected(QListWidgetItem*);
	void hideCurrent();
	void downloadStateChanged(QString);
	void downloadError();
	void deletePlace();
signals:
	void innerBranchSwitched(bool);
};
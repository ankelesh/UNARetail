#pragma once
#include "widgets/parents/inframedWidget.h"
#include "dataproviders/ModesDescriptions.h"
/*
		This widget is made to be connected to totcp\tohttp connections, but no functional is provided now

		TODO: block TCP button

		Update:
			inserted protocols. They are not fully ready, but at least they already are available
			all buttons now MegaIcon
			minor size fixes applied
			constructor modified - now GlobalAppSettings are stored here
		Update:
			Fixed AppSettings sync - now show() is updating address fields.
		Update:
			Added connections between servers: now networking can be tested
		Update:
			now tohttp button is sending instead of downloading
			All is functional except toTcp
		Update:
			removed toTcp renundant buttons etc. Removed awaiting-block if user does not confirmed sending
*/
class QHBoxLayout;
class QVBoxLayout;
class QLabel;
class QVBoxLayout;
class SemaphorLabel;
class MegaIconButton;
class toHttp;
class toLocalMemory;
class QTimer;
class ReceiveWidget : public inframedWidget
{
	Q_OBJECT
private:
	QVBoxLayout* mainLayout;
	SemaphorLabel* semaphor;
	QHBoxLayout* infoLayout;
	QLabel* totalQuantity;
	QLabel* uniqueBarcodes;
	QHBoxLayout* previousDocumentLayout;
	QLabel* numberOfPreviousDocument;
	MegaIconButton* cleanPreviousDocument;
	QLabel* HttpAddress;
	QLabel* LocalAddress;
	MegaIconButton* useHttpButton;
	MegaIconButton* useLocalButton;
	MegaIconButton* backButton;

	toHttp* http;
	toLocalMemory* tolocal;

	bool awaiting;
	Modes currentMode;
	sendingMode mode;
	QTimer* timeoutTimer;
public:
	ReceiveWidget(Modes mode, QWidget* parent);

	virtual void show();
	virtual bool isExpectingControl(int value) override;
	virtual bool giveSettings();
	void setMode(sendingMode);
	void show_info();
	bool sendBySetup();
	void showPrevDoc();
private slots:
	void httpChosen();
	void localChosen();
	void requestEnd(QString data = QString());
	void requestFail(QString stack = QString(),QString message = QString(), int code = 0);
	void requestTimeout();
	void localFail(QString msg);
	void cleanPrevDoc();
signals:
	void sendingSuccess();
	void sendStateChanged(int state);
};

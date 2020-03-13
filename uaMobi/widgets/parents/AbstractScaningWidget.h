#pragma once
#include "widgets/parents/inframedWidget.h"
#include <QtWidgets/qlistwidget.h>
#include <QtWidgets/qscrollbar.h>
#include "widgets/MultibranchWidgets/BarcodeRedactingWidget.h"
#include "widgets/utils/MegaIconButton.h"
#include "widgets/utils/VirtualBarcodeKeyboard.h"
#include "widgets/utils/EventsAndFilters.h"
#include "widgets/MultibranchWidgets/ScaningCameraWidget.h"
#include "widgets/parents/abstractNodeInterface.h"
#include <QtCore/QPointer>
#include <chrono>
#include "widgets/utils/BarcodeObserver.h"


/*
*/
//Performs check of barcode: not too long, not empty
extern bool checkBarcodeIntegrity(const QString& bc);

class AbstractScaningWidget : public inframedWidget, abstractDynamicNode
{
	Q_OBJECT
protected:
	using abstractDynamicNode::untouchable;
	QVBoxLayout* innerLayout;		//	this layout is holding main widgets
	QHBoxLayout* counterLayout;	//	This layout holds shortcuts-> to log and to manual input
	QVBoxLayout* controlLayout;
	QHBoxLayout* buttonLayout;		//	This layout is for buttons necessary for barcode handling
	QHBoxLayout* additionalInputLayout;	//	This layout is turned on if additional input
		//	widgets requested - such as keyboard or camera
	QLabel* modeName;
	QLabel* barcodeInfo;
	QLineEdit* barcodeInput;		//	field for manual barcode input
	MegaIconButton* backButton;		//	emits backRequired
	MegaIconButton* keyboardButton;	//	opens keyboard widget
	MegaIconButton* cameraButton;	//	opens camera widget

	Modes currentMode;

	virtual void _emplaceBarcode(QString barcode) = 0;
	virtual void _clearControls() = 0;
	QString transformModeToString(Modes m);
public:
	AbstractScaningWidget(Modes mode, QWidget* parent = Q_NULLPTR);
	
	virtual void show() override;					//	overrided to provide Input only when needed
	virtual void hide();
protected slots:					//	Slots for inner usage. To submit barcodes etc use tree intefaces
	virtual void handleScanButton() = 0;		//	handles scan press
	virtual void handleValueFromKeyboard(QString value) = 0;
	virtual void barcodeReady() = 0;

	void barcodeConfirmed(QString barcode);
	void cameraRequired();					//	shows camera widget
	void keyboardRequired();				//	shows keyboard
	void hideCurrent();
	void manualConfirmed();
signals:
	void barcodeReceived(Entity);
	void barcodeObtained(QString barcode);
};

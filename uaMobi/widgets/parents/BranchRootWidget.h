#pragma once
#include "submodules/UNAQtCommons/widgets/parents/inframedWidget.h"
#include "submodules/UNAQtCommons/widgets/parents/abstractNodeInterface.h"
#include "Datacore/AbsEntity.h"
#include "dataproviders/ModesDescriptions.h"
/*
	This widget is creating similar interface for branches which differ only by data processing.
	It gives standard branches - scanner, scanned list and receive, so as the settings widget. It is
	fully basing on previously SupplyWidget implementation, carrying it's data processing as a default.
	All slots implemented here must respond to inner branch signals - check connect block of default
	constructor.  Generally, if your branch will differ from supplyWidget only by value of database where data
	will be stored - you can pass corresponding DatabaseNames value to constructor without inheriting this.

	Update:
		minor changes in barcode processing:
		Now barcodes are autocommited.
	Update:
		added back button
	Update:
		Now this is incorporated in inframed's system - it derives key capturing method
		allowing it to use physical shortcuts. Check expectingControls to see more
	Update:
		More abstraction added - now without literals of dbnames
		minor barcode handling fixed - now all barcodes are commited in-time.
	Update:
		Additional vector of barcodes added - it stores sent barcodes
		Changes into refreshing - now additional refresh is provided when barcodes are sent
*/
class QLabel;
class MegaIconButton;
class BranchRootWidget :
	public inframedWidget, abstractDynamicNode
{
	Q_OBJECT
protected:
	QVBoxLayout* innerLayout;
	QLabel* pageName;
	MegaIconButton* scanner;
	MegaIconButton* scanned;
	MegaIconButton* receive;
	MegaIconButton* settings;
	MegaIconButton* backButton;
    using abstractDynamicNode::_hideAndDeleteCurrent;
	Modes currentMode;		//	This is the name of mode

	virtual inframedWidget* _allocateScaning() = 0;
	virtual inframedWidget* _allocateStorage() = 0;
	virtual inframedWidget* _allocateSettings() = 0;
public:
	BranchRootWidget(Modes mode,
		QWidget* parent = Q_NULLPTR);
protected:
	virtual void _numberReaction(int) override;

protected slots:					//	button slots
	void scannerPressed();
	void scannedPressed();
	void receivePressed();
	void settingsPressed();
public slots:					//	newBarcodesArrived is collecting barcodes into Vector
	virtual void backRequire();
	virtual void newBarcodeArrived(Entity);	//	This slot is hiding current and opens widget same as Expecting control
};

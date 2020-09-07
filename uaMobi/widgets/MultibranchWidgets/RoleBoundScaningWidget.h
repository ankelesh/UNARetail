#pragma once
#include "widgets/parents/AbstractScaningWidget.h"
#include <QVector>
#include "widgets/ControlsMiniwidgets/abs_control.h"
#include "Datacore/EntityQuickHash.h"


class MegaIconButton;

namespace labels_private
{
	class abs_counter_label;
}
class TwoLevelCounterLabel;




typedef double(*CounterFillerFuction)(Entity, double);
typedef void (*PostSearchTrigger)(Entity, ShortBarcode, QVector<abs_control*>&,
	QVector<labels_private::abs_counter_label*>&, 
	QTextEdit*);
typedef void (*HistoryCheckTrigger)(Entity, EntityHash, QVector<abs_control*>&,
	DataEntityListModel*, QVector<EntityHash>&);
typedef bool (*PreInsertingTrigger)(Entity, EntityHash, QVector<EntityHash>&, DataEntityListModel*,  QVector<abs_control*>&);


class RoleBoundScaningWidget : public AbstractScaningWidget
{
	Q_OBJECT
protected:
	TwoLevelCounterLabel* lengthInfo;					//	length of barcode
	TwoLevelCounterLabel* totalInfo;					//	how much quantity this barcode has already
	MegaIconButton* okButton;

	QVector<abs_control*> controlList;
	QVector<int> roleBinds;
	QLabel* taxInvoiceInfo;
	QLineEdit* taxInvoiceField;
	Entity pendingBarcode;
	int counterRole;

	QVector<labels_private::abs_counter_label *> counters;
	QVector<CounterFillerFuction> operators;
	PostSearchTrigger pst;
	HistoryCheckTrigger hct;
	PreInsertingTrigger pit;

	uint commentHash;

	QVector<EntityHash> historyUniqueTracking;
	EntityHashingFunction hashingFunction;
	EntityHash pendingBcHash;

	virtual void _emplaceBarcode(QString barcode, ShortBarcode info) override;
	virtual void _clearControls() override;
	virtual void handleValueFromKeyboard(QString value) override;
	virtual void barcodeReady() override;
	virtual bool _validateBarcode(QString) override;
#ifdef CAMERA_SUPPORT
	virtual void handleCameraBarcode(QString value) override;
#endif
	virtual void handleScanButton();
	virtual void _fill_counters();
	virtual void _pushToHistory(Entity) override;
public:
	RoleBoundScaningWidget(Modes mode, int counter_role, QVector<control_bind> bounds, Entity prototype, QWidget* parent);
	void addExtraCounter(QString explanation, CounterFillerFuction f, int type = 0);
	void hideCounter(int which);
	void setPostSearchTrigger(PostSearchTrigger);
	void setHistoryCheckTrigger(HistoryCheckTrigger);
	void setPreInsertingTrigger(PreInsertingTrigger);
	void setTotal(int total);
	void setLen();
protected slots:			//	Slots for inner usage. To submit barcodes etc use tree intefaces
	virtual void okPressed();

};
#pragma once
#include "widgets/parents/BranchRootWidget.h"
#include "widgets/MultibranchWidgets/RoleBoundScaningWidget.h"

class SalesAccountingBranchWidget : public BranchRootWidget
{
	Q_OBJECT
protected:
	// Inherited via BranchRootWidget
	QVector<control_bind> bounds;
	Entity prototype;
    inframedWidget* extraSettings;
    MegaIconButton* extraSettingsButton;
	virtual inframedWidget* _allocateScaning() override;
	virtual inframedWidget* _allocateStorage() override;
	virtual inframedWidget* _allocateSettings() override;
public:
	SalesAccountingBranchWidget(QWidget* parent);
protected slots:
    void extraSettingsRequired();
};

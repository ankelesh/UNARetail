#pragma once
#include "widgets/MultibranchWidgets/Redactors/Abs_Redactor.h"
#include "submodules/UNAQtCommons/widgets/ControlsWidgets/abs_control.h"
#include <QVector>
#include "Datacore/ControlBinds.h"

class RoleBoundRedactor : public Abs_Redactor
{
	Q_OBJECT
protected:
	Entity redacted;
	QVector<abs_control*> controlList;
	QVector<QPair<int, bool> > roleBounds;
	void _clearControlList();
public:
	RoleBoundRedactor(QWidget* parent);
	virtual void setBinds(QVector<control_bind>&);
	virtual bool setRedacted(Entity e) override;
	virtual void extractAndSave() override;
	virtual void applyValues();
};
#pragma once
#include "widgets/MultibranchWidgets/Redactors/Abs_Redactor.h"
#include "widgets/ControlsMiniwidgets/abs_control.h"
#include <QVector>


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
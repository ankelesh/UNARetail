#include "ControlManager.h"
#include "QuantityControl.h"
#include "DateTimeControl.h"
#include "StringControl.h"
#include <QLayout>
#include <QFormLayout>


abs_control* fabricateControl(InputControlEntity::ControlTypes control, QString name, QLayout* layout, QWidget* parent)
{
	abs_control* new_c = Q_NULLPTR;
	switch (control)
	{
	case InputControlEntity::Float:
		new_c = new QuantityControl(false, name, parent);
		break;
	case InputControlEntity::Int:
		new_c = new QuantityControl(true, name, parent);
		break;
	case InputControlEntity::Date:
		new_c = new DateTimeControl(false, name, parent);
		break;
	case InputControlEntity::String:
		new_c = new StringControl(name, parent);
		break;
	case InputControlEntity::Decimals:
		new_c = new QuantityControl(2, name, parent);
		break;
	case InputControlEntity::None:
	default:
		return Q_NULLPTR;
	}
	layout->addWidget(new_c->myWidget());
	return new_c;
}


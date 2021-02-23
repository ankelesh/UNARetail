#include "ControlManager.h"
#include "submodules/UNAQtCommons/widgets/ControlsWidgets/QuantityControl.h"
#include "submodules/UNAQtCommons/widgets/ControlsWidgets/DateTimeControl.h"
#include "submodules/UNAQtCommons/widgets/ControlsWidgets/StringControl.h"
#include <QLayout>
#include <QFormLayout>


abs_control* fabricateControl(abs_control::ControlTypes control, QString name, QLayout* layout, QWidget* parent)
{
	abs_control* new_c = Q_NULLPTR;
	switch (control)
	{
	case abs_control::Float:
		new_c = new QuantityControl(false, name, parent);
		break;
	case abs_control::Int:
		new_c = new QuantityControl(true, name, parent);
		break;
	case abs_control::Date:
		new_c = new DateTimeControl(false, name, parent);
		break;
	case abs_control::String:
		new_c = new StringControl(name, parent);
		break;
	case abs_control::Decimals:
		new_c = new QuantityControl(2, name, parent);
		break;
	case abs_control::None:
	default:
		return Q_NULLPTR;
	}
	layout->addWidget(new_c->myWidget());
	return new_c;
}


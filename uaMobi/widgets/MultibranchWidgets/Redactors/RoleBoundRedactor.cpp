#include "RoleBoundRedactor.h"
#include "Datacore\DataEntities.h"
#include "widgets/ControlsMiniwidgets/ControlManager.h"
#include <QFormLayout>
#include "widgets/utils/GlobalAppSettings.h"
namespace barcode_default_binds
{
	static QVector<control_bind> barcodeBind;
	static QVector<control_bind> pbbind;
	static QVector<control_bind> productbind;
	static uint langhash = 0;
}

void RoleBoundRedactor::_clearControlList()
{
	QVector<abs_control*> ::iterator control = controlList.begin();
	while (control != controlList.end())
	{
		(*control)->deleteLater();
		++control;
	}
	controlList.clear();
}

RoleBoundRedactor::RoleBoundRedactor(QWidget* parent)
	: Abs_Redactor(parent), redacted(), controlList(), roleBounds()
{
	if (barcode_default_binds::langhash != qHash(AppSettings->language))
	{
		barcode_default_binds::barcodeBind << control_bind(abs_control::String, Roles::comment, true, tr("comment"))
			<< control_bind(abs_control::Float, Roles::Barcode::quantity, false, tr("quantity"));
		barcode_default_binds::pbbind << control_bind(abs_control::String, Roles::comment, true, tr("comment"))
			<< control_bind(abs_control::Decimals, Roles::PricedBarcode::general, false, tr("general_price"))
			<< control_bind(abs_control::Decimals, Roles::PricedBarcode::discount, false, tr("discount_price"))
			<< control_bind(abs_control::Decimals, Roles::PricedBarcode::secondary, false, tr("secondary_price"))
			<< control_bind(abs_control::Decimals, Roles::PricedBarcode::unnecessary, false, tr("unnecessary_price"));
		barcode_default_binds::productbind << control_bind(abs_control::String, Roles::comment, true, tr("comment"))
			<< control_bind(abs_control::Float, Roles::Product::quantity, false, tr("quantity"))
			<< control_bind(abs_control::Decimals, Roles::Product::price, false, tr("price"))
			<< control_bind(abs_control::Decimals, Roles::Product::discount, false, tr("discount"));
		barcode_default_binds::langhash = qHash(AppSettings->language);
	}
}

void RoleBoundRedactor::setBinds(QVector<control_bind>& binds)
{
	if (binds.isEmpty())
		return;
	if (!controlList.isEmpty())
		_clearControlList();
	QVector<control_bind>::iterator bind = binds.begin();
	while (bind != binds.end())
	{
		abs_control* new_control = fabricateControl(bind->type, bind->control_name, mainLayout, this);
		if (!controlList.isEmpty())
		{
#ifdef QT_VERSION5X
			QObject::connect(controlList.last(), &abs_control::editingFinished, new_control, &abs_control::setFocus);
#else
			QObject::connect(controlList.last(), SIGNAL(editingFinished()), new_control, SIGNAL(setFocus()));
#endif
		}
		new_control->show();
		controlList << new_control;
		roleBounds << qMakePair(bind->role, bind->is_writeable);
		++bind;
	}
#ifdef QT_VERSION5X
	QObject::connect(controlList.last(), &abs_control::editingFinished, this, &Abs_Redactor::done);
#else
	QObject::connect(controlList.last(), SIGNAL(editingFinished()), this, SIGNAL(done()));
#endif
}

bool RoleBoundRedactor::setRedacted(Entity e)
{
	redacted = e;
	if (e.isNull())
		return false;
	else
	{
		if (roleBounds.isEmpty())
		{
			switch (e->myType())
			{
			case int(barcodeUtil::barcodetypes::uniformBc):
				setBinds(barcode_default_binds::barcodeBind);
				break;
			case int(barcodeUtil::barcodetypes::pricedBc) :
				setBinds(barcode_default_binds::pbbind);
				break;
			case int(barcodeUtil::barcodetypes::product) :
				setBinds(barcode_default_binds::productbind);
				break;
			default:
				return false;
			}
		}
		applyValues();
		return true;
	}
}

void RoleBoundRedactor::extractAndSave()
{
	QVector<QPair<int, bool> > ::iterator bind = roleBounds.begin();
	QVector<abs_control*> ::iterator control = controlList.begin();
	while (bind != roleBounds.end())
	{
		if (bind->second)
		{
			redacted->setWriteable(bind->first, (*control)->getValue());
		}
		else
		{
			redacted->setEnumerable(bind->first,(*control)->getEnumerableValue());
		}
		++bind; ++control;
	}
}

void RoleBoundRedactor::applyValues()
{
	QVector<QPair<int, bool> > ::iterator bind = roleBounds.begin();
	QVector<abs_control*> ::iterator control = controlList.begin();
	while (bind != roleBounds.end())
	{
		if (bind->second)
		{
			(*control)->setValue(redacted->getWriteable(bind->first));
		}
		else
		{
			(*control)->setEnumerableValue(redacted->getEnumerable(bind->first));
		}
		++bind; ++control;
	}
}

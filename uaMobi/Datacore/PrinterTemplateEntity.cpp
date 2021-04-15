#include "PrinterTemplateEntity.h"
#include "dataproviders/TableHandlers.h"
#include <QtSql/qsqlquery.h>
#include <QVariant>
#include "widgets/utils/ElementsStyles.h"
#include <cmath>
#include <QStringBuilder>
#include <QException>

namespace PrinterTemplatePrivate
{
	static QString tableDefinition(QStringLiteral("("
		"id number PRIMARY KEY NOT NULL,"
		"templateName TEXT,"
		"templateText TEXT,"
		"tWidth number,"
		"tHeight number"
		")"));
	QStringList _initTBF()
	{
		QStringList t;
		t <<
			QStringLiteral("id") <<
			QStringLiteral("templateName") <<
			QStringLiteral("templateText") <<
			QStringLiteral("tWidth") <<
			QStringLiteral("tHeight");
		return t;
	}

	static QStringList tableFields(_initTBF());


	static TemplatedTableHandler* barcodeTableHandler(
		new TemplatedTableHandler(
			"PrinterTemplates",
			tableDefinition,
			tableFields,
			0
		)
	);
}

QString PrinterTemplateEntity::_toSql() const
{
	return "(" %
		serializeId() % ",'" %
		templateName % "' , '" %
		templateText % "', " %
		QString::number(width) % " , " %
		QString::number(height) % 
		")";
}

const TemplatedTableHandler* PrinterTemplateEntity::_assocTable() const
{
	return PrinterTemplatePrivate::barcodeTableHandler;
}

QString PrinterTemplateEntity::_formatedView(QString /*sep*/, QString /*dform*/) const
{
	return templateName % "_" % QString::number(width) % "_" % QString::number(height);
}

QString PrinterTemplateEntity::_maximumInfoView(QString /*sep*/, QString /*dform*/) const
{
	return templateText;
}

QString PrinterTemplateEntity::_normalizedCsvView() const
{
	return QString();
}

bool PrinterTemplateEntity::_isValid() const
{
	return (!templateName.isEmpty()) && (!templateText.isEmpty());
}

bool PrinterTemplateEntity::_fromSql(QSqlQuery* q)
{
	if (!q->next())
		return false;
	GUID = q->value(0).toLongLong();
	templateName = q->value(1).toString();
	templateText = q->value(2).toString();
	width = q->value(3).toInt();
	height = q->value(4).toInt();
	return true;
}

AbsEntity* PrinterTemplateEntity::_clone() const
{
	return new PrinterTemplateEntity(*this);
}

QString PrinterTemplateEntity::_getName() const
{
	return templateName;
}

double PrinterTemplateEntity::_getEnumerable(int role) const
{
	switch (role)
	{
	case Roles::PrinterTemplate::width:
		return width;
	case Roles::PrinterTemplate::height:
		return height;
	}
	return 0.0;
}

void PrinterTemplateEntity::_invalidate()
{
	templateName.clear();
	templateText.clear();
	width = 0;
	height = 0;
}

const QStringList& PrinterTemplateEntity::_getFields() const
{
	return PrinterTemplatePrivate::tableFields;
}

QString PrinterTemplateEntity::_fullComparationQuery() const
{
	return QString();
}

void PrinterTemplateEntity::fillPrepQuery(QSqlQuery*q) const
{
	q->bindValue(0, GUID);
	q->bindValue(1, templateName);
	q->bindValue(2, templateText);
	q->bindValue(3, width);
	q->bindValue(4, height);
}

void PrinterTemplateEntity::_setWriteable(int role, QString text)
{
	switch (role)
	{
	case Roles::comment:
		templateName = text;
		break;
	case Roles::PrinterTemplate::templateText:
		templateText = text;
		break;
	}
}

QString PrinterTemplateEntity::_getWriteable(int role) const
{
	switch (role)
	{
	case Roles::comment:
		return templateName;
	case Roles::PrinterTemplate::templateText:
		return templateText;
	}
	return QString();
}

void PrinterTemplateEntity::_erase()
{
	invalidate();
}

int PrinterTemplateEntity::_getFieldNumberForRole(int role) const
{
	switch (role)
	{
	case Roles::common::comment:
		return 1;
	case Roles::PrinterTemplate::templateText:
		return 2;
	case Roles::PrinterTemplate::width:
		return 3;
	case Roles::PrinterTemplate::height:
		return 4;
	default:
		return 0;
	}
}

PrinterTemplateEntity::PrinterTemplateEntity(QString tName, QString tText, int hght, int wdth)
	: AbsEntity(int(barcodeUtil::barcodetypes::printerTemplateNotBC)),
	templateName(tName), templateText(tText), width(wdth), height(hght)
{
}

#include "BarcodeEntity.h"
#include "dataproviders/TableHandlers.h"
#include <QtSql/qsqlquery.h>
#include <QVariant>
#include "widgets/utils/ElementsStyles.h"
#include <cmath>
#include <QStringBuilder>


namespace BarcodeEntityPrivate
{
	static QString tableDefinition(QStringLiteral("("
		"id number PRIMARY KEY NOT NULL,"
		"barcode TEXT,"
		"scanedtime datetime,"
		"quantity number,"
		"uploaded number,"
		"expDateTime datetime,"
		"expComment TEXT,"
		"taxInvoiceNumber TEXT"
		")"));
	QStringList _initTBF()
	{
		QStringList t;
		t <<
			QStringLiteral("id") <<
			QStringLiteral("barcode") <<
			QStringLiteral("scanedtime") <<
			QStringLiteral("quantity") <<
			QStringLiteral("uploaded") <<
			QStringLiteral("expDateTime") <<
			QStringLiteral("expComment") <<
			QStringLiteral("taxInvoiceNumber");
		return t;
	}

	static QStringList tableFields(_initTBF());


	static TemplatedTableHandler* barcodeTableHandler(
		new TemplatedTableHandler(
			"barcodes",
			tableDefinition,
			tableFields,
			0
		)
	);
}


QString BarcodeEntity::_toSql() const
{
	return "(" % serializeId() % ",'" % barcode % "' , '" % addDate.toString(datetimeDBEncoding) 
		% "', " % QString::number(quantity)  % " , " % QString::number(isUploaded) % " , '" 
		% expDate.toString(datetimeDBEncoding)
		 % "' , '" % QString(comment).replace("'", "''").replace("\"", "\"\"") % "','" 
		% taxInvoiceNumber + "')";
}

const TemplatedTableHandler* BarcodeEntity::_assocTable() const
{
	return BarcodeEntityPrivate::barcodeTableHandler;
}

QString BarcodeEntity::_formatedView(QString sep, QString dform) const
{
	QString formated;
	if (!comment.isEmpty())
	{
		formated = comment.leftJustified(15) % sep % QString::number(quantity);
	}
	else
	{
		formated = barcode.leftJustified(15) % sep % QString::number(quantity);
	}
	return formated;
}

QString BarcodeEntity::_maximumInfoView(QString sep, QString dform) const
{
	return barcode % "\n" % sep % QString::number(quantity) % sep %
		addDate.toString(datetimeDBEncoding) % "\n"
		% comment + "\n" % taxInvoiceNumber;
}

QString BarcodeEntity::_normalizedCsvView() const
{
	return barcodeUtil::CSV_BARCODE_STR_TEMPLATE.arg(barcode).arg(addDate.toString()).arg(
		quantity).arg(expDate.toString()).arg(
			comment).arg("").arg("").arg(
				"").arg("").arg("");
}

bool BarcodeEntity::_isValid() const
{
	return quantity != 0 && (!barcode.isEmpty());
}

BarcodeEntity::BarcodeEntity(QString Barcode, QDateTime adddt, int isupl, 
	QDateTime expdt, QString comm, double Quantity, QString tIN)
	: AbsEntity(int(barcodeUtil::barcodetypes::uniformBc)),
	barcode(Barcode), addDate(adddt), isUploaded(isupl),
	expDate(expdt), comment(comm), quantity(Quantity), taxInvoiceNumber(tIN)
{
}

BarcodeEntity::BarcodeEntity(QString bc, QString comm)
: AbsEntity(int(barcodeUtil::barcodetypes::uniformBc)),
barcode(bc), addDate(QDateTime::currentDateTime()), isUploaded(0),
expDate(QDateTime::currentDateTime()), comment(comm), quantity(0),
taxInvoiceNumber()
{

}

unsigned int BarcodeEntity::getEnumerableFieldIndex()
{
	return 3;
}

int BarcodeEntity::_getFieldNumberForRole(int role) const
{
	switch (role)
	{
	case Roles::common::barcode:
		return 1;
	case Roles::common::comment:
		return 6;
	case Roles::common::date:
		return 2;
	case Roles::Barcode::expDate:
		return 5;
	case Roles::Barcode::quantity:
		return 3;
	case Roles::Barcode::taxInvoice:
		return 7;
	case Roles::Barcode::upFlag:
		return 4;
	default:
		return 0;
	}
}

void BarcodeEntity::_setWriteable(int role, QString text)
{
	switch (role)
	{
	case 0:
	case Roles::common::barcode:
		barcode = text;
		break;
	case Roles::common::comment:
		comment = text;
		break;
	case Roles::common::date:
		addDate = QDateTime::fromString(text, datetimeDBEncoding);
		break;
	case Roles::Barcode::expDate:
		expDate = QDateTime::fromString(text, datetimeDBEncoding);
		break;
	case Roles::Barcode::taxInvoice:
		taxInvoiceNumber = text;
		break;
	default: break;

	}
}

QString BarcodeEntity::_getWriteable(int role) const
{
	switch (role)
	{
	case 0:
	case Roles::common::barcode:
		return barcode;
		break;
	case Roles::common::comment:
		return comment;
		break;
	case Roles::common::date:
		return addDate.toString(datetimeDBEncoding);
		break;
	case Roles::Barcode::expDate:
		return expDate.toString(datetimeDBEncoding);
		break;
	case Roles::Barcode::taxInvoice:
		return taxInvoiceNumber;
		break;
	default: break;

	}
}

void BarcodeEntity::_erase()
{
	GUID = AbsEntity::makeGUID();
	barcode.clear();
	addDate = QDateTime::currentDateTime();
	expDate = addDate;
	isUploaded = 0;
	comment.clear();
	quantity = 0;
	taxInvoiceNumber.clear();
}

void BarcodeEntity::_concatenate(const AbsEntity* other)
{
	if (other != Q_NULLPTR)
	{
		if (other->myType() == mytype)
		{
			quantity += other->getEnumerable(Roles::Barcode::quantity);
		}
	}
}

QString BarcodeEntity::_getName() const
{
	return barcode;
}

double BarcodeEntity::_getEnumerable(int role) const
{
	switch (role)
	{
	case 0:
	case Roles::Barcode::Enumerables::quantity:
		return quantity;
	case Roles::Barcode::Enumerables::upFlag:
		return isUploaded;
	default:
		return 0;
	}
}

void BarcodeEntity::_invalidate()
{
	quantity = 0;
}

int BarcodeEntity::_getHeight() const
{
	return (std::ceil(double(barcode.count() + comment.count() 
		+ datetimeDBEncoding.count() 
		)  / double(AppFonts->howMuchCharacterFitsIntoScreen())) + comment.count("\n") + 3);
}

const QStringList& BarcodeEntity::_getFields() const
{
	return BarcodeEntityPrivate::tableFields;
}

QString BarcodeEntity::_fullComparationQuery() const
{
	return "barcode = '" % barcode % "' and quantity = " % QString::number(quantity);
}

void BarcodeEntity::_setEnumerable(int role, double value)
{
	switch (role)
	{
	case 0:
	case Roles::Barcode::quantity:
		quantity = value;
		break;
	case Roles::Barcode::upFlag:
		isUploaded = value;
	default:
		break;
	}
}

void BarcodeEntity::fillPrepQuery(QSqlQuery* q) const
{
	q->bindValue(0, GUID);
	q->bindValue(1, barcode);
	q->bindValue(2, addDate.toString(datetimeDBEncoding));
	q->bindValue(3, quantity);
	q->bindValue(4, isUploaded);
	q->bindValue(5, expDate.toString(datetimeDBEncoding));
	q->bindValue(6, comment);
	q->bindValue(7, taxInvoiceNumber);
}

bool BarcodeEntity::_fromSql(QSqlQuery* q)
{
	if (!q->next())
		return false;
	GUID = q->value(0).toLongLong();
	barcode = q->value(1).toString();
	addDate =
		QDateTime::fromString(q->value(2).toString(), datetimeDBEncoding);
	quantity = q->value(3).toDouble();
	isUploaded = q->value(4).toInt();
	expDate =
		QDateTime::fromString(q->value(5).toString(), datetimeDBEncoding);
	comment = q->value(6).toString();
	taxInvoiceNumber = q->value(7).toString();
	return true;
}

AbsEntity* BarcodeEntity::_clone() const
{
	return new BarcodeEntity(*this);
}

bool BarcodeEntity::deepCompare(AbsEntity* other) const
{
	return AbsEntity::deepCompare(other) && (other->getName() == barcode);
}

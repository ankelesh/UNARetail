#include "ProductEntity.h"
#include "dataproviders/TableHandlers.h"
#include <QtSql/qsqlquery.h>
#include <QVariant>
#include "widgets/utils/ElementsStyles.h"
#include <cmath>
#include <QStringBuilder>
#include <QException>

namespace ProductEntityPrivate
{
	static QString tableDefinition(QStringLiteral("("
		"id number PRIMARY KEY NOT NULL,"
		"barcode TEXT,"
		"scanedtime datetime,"
		"uploaded number,"
		"expComment TEXT,"
		"quantity number,"
		"price number,"
		"discount number"
		")"));
	QStringList _initTBF()
	{
		QStringList t;
		t <<
			QStringLiteral("id") <<
			QStringLiteral("barcode") <<
			QStringLiteral("scanedtime") <<
			QStringLiteral("uploaded") <<
			QStringLiteral("expComment") <<
			QStringLiteral("quantity") <<
			QStringLiteral("price") <<
			QStringLiteral("discount");
		return t;
	}

	static QStringList tableFields(_initTBF());


	static TemplatedTableHandler* barcodeTableHandler(
		new TemplatedTableHandler(
			"products",
			tableDefinition,
			tableFields,
			0
		)
	);
}


QString DEFAULT_RECEIPT_LINE_REPRESENTATION = QStringLiteral("%0__%1*%2 = %3\n");
QString ProductEntity::_toSql() const
{
	return "(" % 
		serializeId() % ",'" % 
		barcode % "' , '" % 
		addDate.toString(DATETIME_ENCODING_FORMAT) % "', " % 
		QString::number(isUploaded) %  " , '" % 
		QString(comment).replace("'", "''").replace("\"", "\"\"")% "' , " % 
		QString::number(quantity) % " , " %
		QString::number(price) % " , " %
		QString::number(discount) % ")";
}

const TemplatedTableHandler* ProductEntity::_assocTable() const
{
	return ProductEntityPrivate::barcodeTableHandler;
}

QString ProductEntity::_formatedView(QString sep, QString /*dform*/) const
{
	QString formated;
	if (!comment.isEmpty())
	{
		formated = comment.leftJustified(15,' ', true) % sep % QString::number(quantity) % '*' + QString::number(price);
	}
	else
	{
		formated = barcode.leftJustified(15,' ', true) % sep % QString::number(quantity) % '*' + QString::number(price);
	}
	return formated;
}

QString ProductEntity::_maximumInfoView(QString sep, QString dform) const
{
	return barcode % "\n" % sep % QString::number(quantity) % sep % QString::number(price) % sep %
		addDate.toString(DATETIME_ENCODING_FORMAT) % "\n"
		% comment;
}

QString ProductEntity::_normalizedCsvView() const
{
	return barcodeUtil::CSV_BARCODE_STR_TEMPLATE.arg(barcode).arg(addDate.toString(DATETIME_ENCODING_FORMAT)).arg(
		quantity).arg("").arg(
			comment).arg(price).arg(discount).arg(
                "").arg("").arg("");
}

QString ProductEntity::_receiptView() const
{
    return DEFAULT_RECEIPT_LINE_REPRESENTATION.arg(comment).arg(price).arg(quantity).arg(price * quantity);
}

bool ProductEntity::_isValid() const
{
	return quantity != 0 && (!barcode.isEmpty());
}

ProductEntity::ProductEntity(QString Barcode,
	QDateTime adddt,
	int isUpl,
	QString comm, double Quantity, double Price, double Discount)
	: AbsEntity(int(barcodeUtil::barcodetypes::product)),
	barcode(Barcode), addDate(adddt), isUploaded(isUpl),
	comment(comm), quantity(Quantity), price(Price), discount(Discount)
{
}

ProductEntity::ProductEntity(QString bc, QString comm)
	: AbsEntity(int(barcodeUtil::barcodetypes::product)),
	barcode(bc), addDate(QDateTime::currentDateTime()), isUploaded(0),
	comment(comm), quantity(0), price(0), discount(0)
{

}

int ProductEntity::_getFieldNumberForRole(int role) const
{
	switch (role)
	{
	case Roles::common::barcode:
		return 1;
	case Roles::common::comment:
		return 4;
	case Roles::common::date:
		return 2;
	case Roles::Product::discount:
		return 7;
	case Roles::Product::quantity:
		return 5;
	case Roles::Product::price:
		return 6;
	case Roles::Product::upFlag:
		return 3;
	default:
		return 0;
	}
}

void ProductEntity::_concatenate(const AbsEntity* other)
{
	if (other != Q_NULLPTR)
		if (other->myType() == mytype)
		{
			quantity += other->getEnumerable(Roles::Product::quantity);
		}
}

void ProductEntity::_setWriteable(int role, QString text)
{
	switch (role)
	{
	case 0:
	case Roles::common::barcode:
		barcode = text;
		break;
	case 1:
	case Roles::common::date:
		addDate = QDateTime::fromString(text, DATETIME_ENCODING_FORMAT);
		break;
	case 2:
	case Roles::common::comment:
		comment = text;
		break;
	default:
		break;
	}
}

QString ProductEntity::_getWriteable(int role) const
{
	switch (role)
	{
	case 0:
	case Roles::common::barcode:
		return barcode;
		break;
	case 1:
	case Roles::common::date:
		return addDate.toString(DATETIME_ENCODING_FORMAT);
		break;
	case 2:
	case Roles::common::comment:
		return comment;
		break;
	default:
		return QString();
	}
}

void ProductEntity::_erase()
{
	GUID = AbsEntity::makeGUID();
	barcode.clear();
	addDate = QDateTime::currentDateTime();
	isUploaded = 0;
	comment.clear();
	price = 0;
	discount = 0;
	quantity = 0;
}


QString ProductEntity::_getName() const
{
	return barcode;
}

double ProductEntity::_getEnumerable(int role) const
{
	switch (role)
	{
	case 0:
	case Roles::Product::quantity:
		return double(quantity);
	case Roles::Product::price:
		return price;
	case Roles::Product::discount:
		return discount;
	case Roles::Product::upFlag:
		return isUploaded;
	default:
		return 0;
	}
}

void ProductEntity::_invalidate()
{
	quantity = 0;
	price = 0;
	discount = 0;
}

int ProductEntity::_getHeight() const
{
	return (std::ceil(double(barcode.count() + comment.count()
		+ DATETIME_ENCODING_FORMAT.count()
		) / double(AppFonts->howMuchCharacterFitsIntoScreen())) + comment.count("\n") + 2);
}

const QStringList& ProductEntity::_getFields() const
{
	return ProductEntityPrivate::tableFields;
}

QString ProductEntity::_fullComparationQuery() const
{
	return "barcode = '" % barcode % "' and quantity = " % QString::number(quantity);
}

void ProductEntity::_setEnumerable(int role, double value)
{
	switch(role)
	{
	case Roles::Product::quantity:
		quantity = value;
		break;
	case Roles::Product::price:
		price = value;
		break;
	case Roles::Product::discount:
		discount = value;
		break;
	case Roles::Product::upFlag:
		isUploaded = int(value);
		break;
	default:
		break;
	}
}

void ProductEntity::fillPrepQuery(QSqlQuery* q) const
{
	q->bindValue(0, GUID);
	q->bindValue(1, barcode);
	q->bindValue(2, addDate.toString(DATETIME_ENCODING_FORMAT));
	q->bindValue(3, isUploaded);
	q->bindValue(4, comment);
	q->bindValue(5, quantity);
	q->bindValue(6, price);
	q->bindValue(7, discount);
}

bool ProductEntity::_fromSql(QSqlQuery* q)
{
	if (!q->next())
		return false;
	GUID = q->value(0).toLongLong();
	barcode = q->value(1).toString();
	addDate = QDateTime::fromString(q->value(2).toString(), DATETIME_ENCODING_FORMAT);
	isUploaded = q->value(3).toInt();
	comment = q->value(4).toString();
	quantity = q->value(5).toDouble();
	price = q->value(6).toDouble();
	discount = q->value(7).toDouble();
	return true;
}

AbsEntity* ProductEntity::_clone() const
{
	return new ProductEntity(*this);
}

bool ProductEntity::deepCompare(AbsEntity* other) const
{
	return AbsEntity::deepCompare(other) && (other->getName() == barcode);
}

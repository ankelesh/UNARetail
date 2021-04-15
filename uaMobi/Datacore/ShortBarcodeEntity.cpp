#include "ShortBarcodeEntity.h"
#include <QtSql/QSqlQuery>
#include <QVariant>
#include <QStringBuilder>
#include "dataproviders/TableHandlers.h"


namespace ShortBarcodePrivate {
	static QString tableDefinition(QStringLiteral("("
		"id number PRIMARY KEY NOT NULL,"
		"barcode TEXT,"
		"code number,"
		"info TEXT,"
		"count TEXT,"
		"price number,"
        "discprice number,"
        "nameAlt TEXT,"
        "unitPrice TEXT"
		")"));

	QStringList _initTSBF()
	{
		QStringList t;
		t <<
			QStringLiteral("id") <<
			QStringLiteral("barcode") <<
			QStringLiteral("code") <<
			QStringLiteral("info") <<
			QStringLiteral("count") <<
			QStringLiteral("price") <<
            QStringLiteral("discprice") <<
            QStringLiteral("nameAlt")<<
            QStringLiteral("unitPrice");
		return t;
	}
	static QStringList tableFields(_initTSBF());


	static TemplatedTableHandler* barcodeTableHandler(
		new TemplatedTableHandler(
			"shortbcs",
			tableDefinition,
			tableFields,
			0
		)
	);

}

ShortBarcodeEntity::ShortBarcodeEntity(QString Barcode, int cod, QString Info, QString Count, double Price, double Discount)
	: AbsEntity(int(barcodeUtil::barcodetypes::shortBc)), barcode(Barcode), code(cod),
	info(Info), count(Count), price(Price), discount(Discount)
{
}

void ShortBarcodeEntity::clear()
{
	barcode.clear();
	code = 0;
	info.clear();
	count.clear();
	GUID = makeGUID();
	price = 0.0;
}

const TemplatedTableHandler* ShortBarcodeEntity::getTableHandler()
{
	return ShortBarcodePrivate::barcodeTableHandler;
}

const QString ShortBarcodeEntity::makeInsertionFromString(QString vals)
{
	return ShortBarcodePrivate::barcodeTableHandler->insert("(" % QString::number(AbsEntity::makeGUID()) 
		% ","%vals%")");
}

const QStringList & ShortBarcodeEntity::getFieldNames()
{
	return ShortBarcodePrivate::tableFields;
}

QSharedPointer<ShortBarcodeEntity> ShortBarcodeEntity::extractFromLine(QString line)
{
	QSharedPointer<ShortBarcodeEntity> toReturn(new ShortBarcodeEntity());
	QStringList split2lvl = line.split(",");
	switch (split2lvl.count())
	{
    case 8:
        if (!split2lvl.at(7).isEmpty())
            toReturn->unitPrice = split2lvl.at(7).toDouble();
        Q_FALLTHROUGH();
    case 7:
        if (!split2lvl.at(6).isEmpty())
            toReturn->nameAlt = split2lvl.at(6);
        Q_FALLTHROUGH();
	case 6:
        if (!split2lvl.at(5).isEmpty())
            toReturn->discount = split2lvl.at(5).toDouble();
		Q_FALLTHROUGH();
	case 5:
        if (!split2lvl.at(4).isEmpty())
            toReturn->price = split2lvl.at(4).toDouble();
		Q_FALLTHROUGH();
	case 4:
        if (!split2lvl.at(3).isEmpty())
            toReturn->count = split2lvl.at(3);
		Q_FALLTHROUGH();
	case 3:
        if (!split2lvl.at(2).isEmpty())
            toReturn->info = split2lvl.at(2);
		Q_FALLTHROUGH();
	case 2:
        if (!split2lvl.at(1).isEmpty())
            toReturn->code = split2lvl.at(1).toInt();
		Q_FALLTHROUGH();
	case 1:
		toReturn->barcode = split2lvl.at(0);
		Q_FALLTHROUGH();
	default:
		break;
	}
	if (toReturn->barcode.isEmpty())
	{
		toReturn->barcode.fill('1', 1);
	}
	return toReturn;
}

int ShortBarcodeEntity::_getFieldNumberForRole(int role) const
{
	switch (role)
	{
	case Roles::common::barcode:
		return 1;
	case Roles::ShortBarcode::Enumerables::code:
		return 2;
	case Roles::ShortBarcode::Enumerables::discount:
		return 6;
	case Roles::ShortBarcode::count:
		return 4;
	case Roles::ShortBarcode::price:
		return 5;
	case Roles::comment:
		return 3;
    case Roles::ShortBarcode::nameAlt:
        return 7;
    case Roles::ShortBarcode::unitPrice:
        return 8;
	default:
		return 0;
	}
}

void ShortBarcodeEntity::_setWriteable(int role, QString text)
{
	switch (role)
	{
	case 0:
	case Roles::common::barcode:
		barcode = text;
		break;
	case Roles::common::comment:
		info = text;
		break;
	case Roles::ShortBarcode::Writeables::count:
		count = text;
		break;
    case Roles::ShortBarcode::nameAlt:
        nameAlt = text;
        break;
	default: break;
	}
}

QString ShortBarcodeEntity::_getWriteable(int role) const
{
	switch (role)
	{
	case 0:
	case Roles::common::barcode:
		return barcode;
		
	case Roles::common::comment:
		return info;
		
	case Roles::ShortBarcode::Writeables::count:
		return count;
    case Roles::ShortBarcode::Writeables::nameAlt:
        return nameAlt;
	default: return QString();
	}
}

void ShortBarcodeEntity::_erase()
{
	clear();
}

QString ShortBarcodeEntity::_getName() const
{
	return barcode;
}

double ShortBarcodeEntity::_getEnumerable(int role) const
{
	switch (role)
	{
	case 0:
	case Roles::ShortBarcode::price:
		return price;
	case Roles::ShortBarcode::code:
		return code;
	case Roles::ShortBarcode::discount:
		return discount;
    case Roles::ShortBarcode::unitPrice:
        return unitPrice;
	default:
		return 0;
	}
}

void ShortBarcodeEntity::_setEnumerable(int role, double value)
{
	switch (role)
	{
	case 0:
	case Roles::ShortBarcode::price:
		price = value;
		break;
	case Roles::ShortBarcode::code:
		code = value;
		break;
	case Roles::ShortBarcode::discount:
		discount = value;
		break;
    case Roles::ShortBarcode::unitPrice:
        unitPrice = value;
        break;
	default:
		break;
	}
}

bool ShortBarcodeEntity::deepCompare(AbsEntity* other) const
{
	return AbsEntity::deepCompare(other) && other->getName() == barcode;
}

void ShortBarcodeEntity::_invalidate()
{
	barcode.clear();
}

const QStringList& ShortBarcodeEntity::_getFields() const
{
	return ShortBarcodePrivate::tableFields;
}

QString ShortBarcodeEntity::_fullComparationQuery() const
{
	return "barcode = '" % barcode % "'";
}

void ShortBarcodeEntity::fillPrepQuery(QSqlQuery*q) const
{
	q->bindValue(0, GUID);
	q->bindValue(1, barcode);
	q->bindValue(2, code);
	q->bindValue(3, info);
	q->bindValue(4, count);
	q->bindValue(5, price);
	q->bindValue(6, discount);
    q->bindValue(7, nameAlt);
    q->bindValue(8, unitPrice);
}

QString ShortBarcodeEntity::_toSql() const
{
	return "( " % serializeId() % ",'" % barcode % "'," % QString::number(code) % ",'" % info % "','"
        % count %"'," % QString::number(price)% "," % QString::number(discount) % ",'"%nameAlt%"',"%QString::number(unitPrice)%")";
}

const TemplatedTableHandler* ShortBarcodeEntity::_assocTable() const
{
	return ShortBarcodePrivate::barcodeTableHandler;
}

QString ShortBarcodeEntity::_formatedView(QString sep, QString /*dform*/) const
{
	return barcode % sep % info % sep % QString::number(code);
}

QString ShortBarcodeEntity::_maximumInfoView(QString sep, QString /*dform*/) const
{
	return QString::number(price) % sep % QString::number(discount)
		% sep % info;
}

QString ShortBarcodeEntity::_normalizedCsvView() const
{
	return barcodeUtil::CSV_BARCODE_STR_TEMPLATE.arg(barcode).arg("").arg(
		count).arg(QString()).arg(QString::number(price) % " " %
			info).arg("").arg("").arg(
				"").arg("").arg(code);
}

bool ShortBarcodeEntity::_isValid() const
{
	return !barcode.isEmpty();
}

bool ShortBarcodeEntity::_fromSql(QSqlQuery* q)
{
	if (!q->next())
		return false;
	GUID = q->value(0).toLongLong();
	barcode = q->value(1).toString();
	code = q->value(2).toInt();
	info = q->value(3).toString();
	count = q->value(4).toString();
	price = q->value(5).toDouble();
	discount = q->value(6).toDouble();
    nameAlt = q->value(7).toString();
    unitPrice = q->value(8).toDouble();
	return true;
}

AbsEntity* ShortBarcodeEntity::_clone() const
{
	return new ShortBarcodeEntity(*this);
}

#pragma once
#include "AbsEntity.h"
#include <QDateTime>

namespace Roles
{
	namespace Product {
		enum Enumerables {
			quantity = 64,
			price,
			discount,
			upFlag
		};
	}
}
class ProductEntity : public AbsEntity
{
protected:
	// Inherited via AbsEntity
	virtual QString _toSql() const override;
	virtual const TemplatedTableHandler* _assocTable() const override;
	virtual QString _formatedView(QString sep, QString dform) const override;
	virtual QString _maximumInfoView(QString sep, QString dform) const override;
	virtual QString _normalizedCsvView() const override;
	virtual bool _isValid() const override;
	virtual bool _fromSql(QSqlQuery*) override;
	virtual AbsEntity* _clone() const override;
	virtual bool deepCompare(AbsEntity*) const override;
	virtual QString _getName() const override;
	virtual double _getEnumerable(int role) const override;
	virtual void _invalidate() override;
	virtual int _getHeight() const override;
	virtual const QStringList& _getFields() const override;
	virtual QString _fullComparationQuery() const override;
	virtual void _setEnumerable(int role, double value) override;
	virtual void fillPrepQuery(QSqlQuery*) const override;
	virtual void _setWriteable(int role, QString text) override;
	virtual QString _getWriteable(int role) const override;
	virtual void _erase() override;
	virtual int _getFieldNumberForRole(int role) const override;
	virtual void _concatenate(const AbsEntity* other) override;
public:

	QString barcode;			//	String representation of barcode. Is used to distinct one barcode from another.
	QDateTime addDate;		//	datetime when barcode was scanned. If it was scanned multiple times - only first stays
	int isUploaded;
	QString comment;		//	comment to barcode
	double quantity;
	double price;
	double discount;
	explicit ProductEntity(QString barcode = QString(),
		QDateTime adddt = QDateTime::currentDateTime(),
		int isUpl = 0,
		QString comm = QString(), double Qty = 0, double price=0, double discount = 0);
	explicit ProductEntity(QString bc, QString comm);



};

typedef QSharedPointer<ProductEntity> Product;
typedef QVector<Product> ProductList;
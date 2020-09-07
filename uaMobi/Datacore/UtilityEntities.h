#pragma once
#include "AbsEntity.h"




class SeparatorEntity : public AbsEntity
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
	virtual QString _getName() const override;
	virtual double _getEnumerable(int role) const override;
	virtual void _invalidate() override;
	virtual const QStringList& _getFields() const override;
	virtual QString _fullComparationQuery() const override;
	virtual void fillPrepQuery(QSqlQuery*) const override;
	// Inherited via AbsEntity
	virtual int _getFieldNumberForRole(int role) const override;

	// Inherited via AbsEntity
	virtual void _setWriteable(int role, QString text) override;
	virtual QString _getWriteable(int role) const override;
	virtual void _erase() override;
public:
	SeparatorEntity();
	virtual ~SeparatorEntity();

};
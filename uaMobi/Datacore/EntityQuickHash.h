#pragma once
#include "AbsEntity.h"
#include <QHash>



class EntityHash
{
	uint barcodeHash;
	int type;
	long long int enumerablesHash;
	uint writeablesHash;
public:
	EntityHash() : barcodeHash(0), type(-1), enumerablesHash(0), writeablesHash(0) {}
	EntityHash(uint bHash, int t, long long int eH = 0, uint wH = 0)
		: barcodeHash(bHash), type(t), enumerablesHash(eH), writeablesHash(wH) {}
	EntityHash& operator=(const EntityHash& other);
	bool operator==(const EntityHash& other) const;
	bool operator<(const EntityHash& other) const;
	void clear();
	friend uint qHash(const EntityHash& h);
};
inline EntityHash quickHash(const Entity& e)
{
	return EntityHash(qHash(e->getName()), e->myType());
}
template <class PointerToEntity>
inline EntityHash quickHash(const PointerToEntity& pe)
{
	return EntityHash(qHash(pe->getName()), pe->myType());
}
typedef EntityHash(*EntityHashingFunction)(const Entity&);
inline uint qHash(const EntityHash& h)
{
	return h.barcodeHash;
}
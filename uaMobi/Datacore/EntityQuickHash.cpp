#include "EntityQuickHash.h"

EntityHash& EntityHash::operator=(const EntityHash& other)
{
	barcodeHash = other.barcodeHash;
	type = other.type;
	enumerablesHash = other.enumerablesHash;
	writeablesHash = other.writeablesHash;
	return *this;
}

bool EntityHash::operator==(const EntityHash& other) const
{
	return (
		(type == other.type)
		&&
		(barcodeHash == other.barcodeHash)
		&&
		(enumerablesHash == other.enumerablesHash)
		&&
		(writeablesHash == other.writeablesHash)
		);
}

bool EntityHash::operator<(const EntityHash& other) const
{
	return (
		(type < other.type)
		||
		( barcodeHash > other.barcodeHash)
		);
}

void EntityHash::clear()
{
	type = 0;
	barcodeHash = 0;
	enumerablesHash = 0;
	writeablesHash = 0;
}

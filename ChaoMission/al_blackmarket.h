#pragma once
#include "cwe_api.h"


struct BlackMarketCategoryAttribute
{
	BlackMarketItemAttributes* attrib;
	int Count;
};

DataArray(BlackMarketCategoryAttribute, CategoryAttribs, 0x8A8728, 9);

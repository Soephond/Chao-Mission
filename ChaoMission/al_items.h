#pragma once

#include "pch.h"
#include "al_items_structs.h"
#include "KCE_Helper.h"

DataArray(MinimalData, MinimalsPresent, 0x19F6B18, 10);
DataArray(FruitData, FruitsPresent, 0x19F6528, 24);
DataArray(HatData, HatsPresent, 0x19F6938, 24);
DataArray(SeedData, SeedsPresent, 0x019F6848, 12);

bool AddMinimalToGarden(short type, int amount);
bool AddItemToGarden(ChaoItemCategory category, short type, int amount);
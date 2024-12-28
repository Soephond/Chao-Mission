#pragma once

#include "pch.h"
#include "KCE_Helper.h"

DataPointer(animalInv, HeroAnimalInv, 0x1946EC8);
DataPointer(animalInv, DarkAnimalInv, (0x1946EC8 + sizeof(animalInv)));
FastcallFunctionPointer(animalInv*, GetAnimalInventory, (int pno), 0x5A6F50);

bool AddMinimalsToInventory(int pno, int type, int amount);
bool AddItemsToInventory(ChaoItemCategory category, short type, int amount);

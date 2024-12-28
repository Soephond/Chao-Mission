#include "pch.h"
#include "al_inv.h"

#include <iostream>

bool AddMinimalsToInventory(int pno, int type, int amount)
{
    std::cout << "Adding " << amount << " animals of type " << type << " to player " << pno << std::endl;
    animalInv* inv = (pno % 2) ? &DarkAnimalInv : &HeroAnimalInv;
    if (inv->Count + amount <= 10 && inv->Count < 10)
    {
        for (int i = 0; i < amount; i++)
        {
            inv->Set2[inv->Count].type = type;
            inv->Set2[inv->Count].timer = 0;
            inv->Count++;
        }
        return true;
    }
    return false;
}

bool AddItemsToInventory(ChaoItemCategory category, short type, int amount)
{
    SAlItem* itemInventory = KCEptr->pPurchasedItems;
    int* purchasedAmount = PurchasedAmount();
    if (*purchasedAmount + amount <= 10 && *purchasedAmount < 10)
    {
        for (int i = 0; i < amount; i++)
        {
            itemInventory[*purchasedAmount].mCategory = category;
            itemInventory[*purchasedAmount].mType = type;
            *purchasedAmount += 1;
        }
        return true;
    }
    return false;
}

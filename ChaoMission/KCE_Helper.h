#pragma once
#include "pch.h"
#include "al_inv_structs.h"
#include "MemAccess.h"

struct KCE_HELPER
{
    const char** pCustomChaoMap;
    size_t CustomChaoMapSize; //pair count

    const char** pMsgAlItem;
    size_t MsgAlItemSize;

    char* pCweSave;

    SAlItem* pPurchasedItems;

    int* pLensSpecialMap;
    size_t LensSpecialMapSize;
};

DataPointer(KCE_HELPER*, KCEptr, 0x53FF04);

int* PurchasedAmount();

#include "KCE_Helper.h"

int* PurchasedAmount()
{
    return (int*)(KCEptr->pCweSave + 0xCBAC);
}

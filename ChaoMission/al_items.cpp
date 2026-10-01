#include "pch.h"
#include "al_items.h"

#include <iostream>

template <auto NoneValue, typename Container>
int AmountOfFilledSlots(Container& arr) 
{
    int amount = 0;
    for (int i = 0; i < arr.size(); i++) {
        if (arr[i].Type != NoneValue) amount++;
    }
    
    return amount;
}

template <auto NoneValue, typename Container>
int GetFreeSlot(Container& arr) 
{
    for (int i = 0; i < arr.size(); i++) {
        if (arr[i].Type == NoneValue) return i;
    }
    
    return -1;
}

template <auto NoneValue, typename Container>
bool AddToGarden(Container& arr, decltype(NoneValue) type, unsigned int amount)
{
    int curAmount = AmountOfFilledSlots<NoneValue>(arr);
    
    if (curAmount + amount > arr.size() || curAmount >= arr.size())
    {
        return false;
    }
    
    for (int i = 0; i < amount; i++)
    {
        int freeIndex = GetFreeSlot<NoneValue>(arr);
        
        if (freeIndex != -1)
        {
            arr[freeIndex].Type = type;
        }
    }
    
    return true;
}

bool AddMinimalToGarden(short type, int amount)
{
    std::cout << "Adding " << amount << " animals of type " << type << " to garden " << GetCurrentChaoStage() << "\n";
    
    return AddToGarden<Al_Animal_None>(MinimalsPresent, static_cast<Al_Animal>(type), amount);
}

bool AddItemToGarden(ChaoItemCategory category, short type, int amount)
{
    switch(category)
    {
        case ChaoItemCategory_Egg:
        {
            
        }
        
        case ChaoItemCategory_Hat:
        {
            
        }
        
        default:
            throw std::runtime_error("AddItemToGarden: unknown category");
    }
}


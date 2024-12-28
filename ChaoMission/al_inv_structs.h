#pragma once

#include "pch.h"

struct SAlItem
{
    Sint8 mCategory;
    Uint16 mType;
};

struct animalInvPair
{
    int type;
    int timer;
};

struct animalInv
{
    animalInvPair Set1[10];
    animalInvPair Set2[10];

    int Count;
};
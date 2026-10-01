#pragma once

#include "pch.h"
#include "chao_data.h"

struct MinimalData
{
    Al_Animal Type;
    SA2BChaoGarden Garden;
    byte Undef[16];
};

struct FruitData
{
    AL_Fruit Type;
    SA2BChaoGarden Garden;
    short Size;
    Uint8 Age;
    byte Undef[12];
};

struct HatData
{
    AL_Hat Type;
    SA2BChaoGarden Garden;
    byte Undef[16];
};

struct SeedData
{
    ChaoSeed Type;
    SA2BChaoGarden Garden;
    byte Undef[16];
};
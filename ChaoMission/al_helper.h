#pragma once

#include "MemAccess.h"

#pragma pack(push, 8)
struct RingDisplayData
{
	float posX;
	float posY;
	int field_8;
	int field_C;
	int field_10;
	int field_14;
	__int16 field_18;
	char field_1A;
	char field_1B[7];
	char field_22[7];
	char field_29;
	char field_2A;
	char field_2B;
};
#pragma pack(pop)


char* DecodeChaoName(char Name[]);

//void __usercall RingDisplay_AddRings(signed int a1@<eax>)
const int sub_5A6B30 = 0x5A6B30;
void RingDisplay_AddRings(signed int a1);

DataPointer(RingDisplayData, RingDisplayData_, 0x1A27870);

FunctionPointer(void, LoadRingDisplay, (float a1, float a2), 0x5A6AC0);
VoidFunc(DeleteRingDisplay, 0x5A6B10);
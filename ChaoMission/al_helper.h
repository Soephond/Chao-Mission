#pragma once

#include "al_chao_home.h"
#include "MemAccess.h"
#include <array>

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


bool DecodeChaoName(wchar_t Name[], wchar_t* ChaoNameDecoded, int size = 7);
bool EncodeChaoName(wchar_t Name[], wchar_t* ChaoNameEncoded, int size = 7);
bool EncodeChaoNameFromString(const char* Name, char* ChaoNameEncoded, int size = 7);
NJS_POINT3 (*GetSetPosGarden(eCHAO_STAGE_NUMBER stage))[16];

//void __usercall RingDisplay_AddRings(signed int a1@<eax>)
const int sub_5A6B30 = 0x5A6B30;
void RingDisplay_AddRings(signed int a1);

//void __usercall AL_GeneAnalyzeCommon(ChaoDNA *a1@<edi>, ChaoDataBase *a2)
const int sub_551DA0 = 0x551DA0;
void AL_GeneAnalyzeCommon(ChaoDNA* a1, ChaoDataBase* a2);

DataPointer(RingDisplayData, RingDisplayData_, 0x1A27870);

FunctionPointer(void, LoadRingDisplay, (float a1, float a2), 0x5A6AC0);
VoidFunc(DeleteRingDisplay, 0x5A6B10);

FunctionPointer(ChaoData*, GetFreeChaoSlot, (), 0x531AA0);
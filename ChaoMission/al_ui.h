#pragma once

#include "cwe_api.h"
#include "al_msg.h"

#include "pch.h"

extern "C"
{
	//void __usercall DrawChaoHudThingB(ChaoHudThingB* a1@<eax>, float a2, float a3, float a4, float a5, float a6, int a7, int a8)
	const int DrawChaoHudThingBptr = 0x00558BE0;
	void DrawChaoHudThingB(ChaoHudThingB* a1, float a2, float a3, float a4, float a5, float a6, int a7, int a8);
}

FunctionPointer(void, AL_CreateChoose, (float a1, float a2, float a3), 0x57EE80);
FunctionPointer(void, SetChaoHUDThingBColor, (float a1, float a2, float a3, float a4), 0x00559FA0);
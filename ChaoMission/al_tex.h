#pragma once
#include "ninja.h"

extern NJS_TEXNAME AL_ODE_CHAO_TEXNAME[2];
extern NJS_TEXLIST AL_ODE_CHAO_TEXLIST[];

//void __usercall LoadChaoTexlist(char* a2@<ebx>, NJS_TEXLIST* texlist, unsigned __int16 a1@<ax>)
const int sub_530280 = 0x530280;
void LoadChaoTexlist(const char* a2, NJS_TEXLIST* texlist, unsigned __int16 a1);

NJS_TEXLIST* GetCHAO_TEXLIST();
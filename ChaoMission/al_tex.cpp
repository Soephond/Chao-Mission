#include "al_tex.h"

NJS_TEXNAME AL_ODE_CHAO_TEXNAME[2];
NJS_TEXLIST AL_ODE_CHAO_TEXLIST[] = { AL_ODE_CHAO_TEXNAME, 2 };

void LoadChaoTexlist(const char* a2, NJS_TEXLIST* texlist, unsigned __int16 a1)
{
	__asm
	{
		mov ax, [a1]
		mov ebx, [a2]
		push[texlist]
		call sub_530280
		add esp, 4
	}
}

NJS_TEXLIST* GetCHAO_TEXLIST()
{
	return AL_ODE_CHAO_TEXLIST;
}
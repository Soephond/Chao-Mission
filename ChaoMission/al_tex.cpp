#include "al_tex.h"

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
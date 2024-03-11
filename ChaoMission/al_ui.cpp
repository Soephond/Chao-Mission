#include "al_ui.h"

extern "C"
{
#pragma optimize( "gty", off )
	//void __usercall DrawChaoHudThingB(ChaoHudThingB* a1@<eax>, float a2, float a3, float a4, float a5, float a6, int a7, int a8)
	void DrawChaoHudThingB(ChaoHudThingB* a1, float a2, float a3, float a4, float a5, float a6, int a7, int a8)
	{
		__asm
		{
			push[a8]
			push[a7]
			push[a6]
			push[a5]
			push[a4]
			push[a3]
			push[a2]
			mov eax, a1
			call DrawChaoHudThingBptr
			add esp, 7 * 4
		}
	}
#pragma optimize( "gty", on )
}
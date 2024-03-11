#pragma once

#include "pch.h"

#include "SA2Structs.h"
#include "MemAccess.h"

#pragma pack(push, 8)
	struct Message380
	{
		_DWORD Mode;
		Buttons ButtonPress;
		int ButtonOn;
		float field_C;
		float field_10;
		float field_14;
		_DWORD dword_18;
	};
#pragma pack(pop)

#pragma pack(push, 8)
	struct Message380Array
	{
		Message380 array[32];
	};
#pragma pack(pop)

#pragma pack(push, 8)
	struct __declspec(align(4)) MessageFontThing
	{
		signed __int16 signed0;
		__int16 mSizeX;
		__int16 field_4;
		__int16 field_6;
		signed __int16* psigned8;
	};
#pragma pack(pop)


#pragma pack(push, 8)
	struct MessageField0
	{
		MessageFontThing* dword_0;
		float float_4;
		float float_8;
		float xSize;
		float ySize;
		int field_14;
		float field_18;
	};
#pragma pack(pop)

#pragma pack(push, 8)
	struct MessageField0Array
	{
		MessageField0 array[9];
	};
#pragma pack(pop)


#pragma pack(push, 8)
	struct KinderCoMessageThing
	{
		MessageField0Array* gap0;
		Message380Array* pointerToSomething0x380;
		__int16 someKindaCount;
		__int16 wordsLeftMaybe;
		float field_C;
		float posX;
		float posY;
		float rectSizeX;
		float rectSizeY;
		float field_20;
		float field_24;
		__int16 field_28;
		char enabled;
		char field_2B;
		int ContinueDotColor;
	};
#pragma pack(pop)

#pragma pack(push, 8)
	struct Al_MSGWarnKinderMessageThing
	{
		char uninitialized;
		char field_1;
		char field_2;
		char field_3;
		float field_4;
		float field_8;
		float field_C;
		float field_10;
		KinderCoMessageThing* pkindercomessagething14;
	};
#pragma pack(pop)

#pragma pack(push, 8)
	struct __declspec(align(4)) AL_MSGWarnData
	{
		char field_0;
		char field_1;
		char field_2;
		char field_3;
		float field_4;
		float field_8;
		float field_C;
		float field_10;
		MessageFontThing field_14;
		MessageFontThing field_20;
		MessageFontThing field_2C;
		MessageFontThing field_38;
	};
#pragma pack(pop)

	DataArray(Al_MSGWarnKinderMessageThing, Al_MSGWarnKinderMessageArray, 0x1A259E8, 4);
	DataPointer(AL_MSGWarnData, SomeAL_MSGWarn, 0x1A276D8);
	DataPointer(int, dword_1A267D0, 0x1A267D0);

	FunctionPointer(Message380*, sub_543860, (), 0x543860);
	FunctionPointer(void, AlMsgWarnCreate, (float a1, float a2, float a3, float a4), 0x5436F0);
	VoidFunc(AlMsgWarnOpen, 0x543800);
	FastcallFunctionPointer(void, AlMsgWarnAddLineC, (int a1), 0x52DC90);
	VoidFunc(AlMsgWarnRemove, 0x5437B0);
	VoidFunc(sub_52FB80, 0x52FB80);
	FunctionPointer(KinderCoMessageThing*, AlMsgWarnClear, (), 0x543830);

extern "C" {
	//void __usercall ALMsgWinAddLineC(KinderCoMessageThing* a2@<esi>, const char* a1@<ebx>, bool a3)
	const int sub_579E30Ptr = 0x579E30;
	void AlMsgWinAddLineC(KinderCoMessageThing* a2, const char* a1, bool a3);
}
#pragma once

#include "SA2Structs.h"
enum ODE_MENU_STATE
{
	ODE_MENU_STATE_OK = 0,
	ODE_MENU_STATE_BUSY = 1,
	ODE_MENU_STATE_END = 2,
	ODE_MENU_STATE_FAIL = -1
};

struct ODE_MENU_MASTER_WORK
{
	int PreStage;
	int CurrStage;
	int NextStage;
	int mode;
	int timer;
	int subtimer;
	int counter;
	ODE_MENU_STATE state;
	int EndFlag;
	int cursorX;
	int cursorY;
	ObjectMaster* tp;
	void* mpStageWork;
	void(__cdecl* mfStageExit)(ODE_MENU_MASTER_WORK*);
};

DataPointer(ODE_MENU_STATE, MenuStatus, 0x1AED254);
DataPointer(ODE_MENU_MASTER_WORK*, AL_OdekakeMenuMaster_Data_ptr, 0x1AED324);
DataPointer(int, dword_1A557C8, 0x1A557C8);

FastcallFunctionPointer(CHAO_PARAM_GC*, AL_GBAManagerGetChaoData, (), 0x532D00);
FunctionPointer(ODE_MENU_MASTER_WORK*, AL_OdeMenuChangeStage, (), 0x57E680);
FastcallFunctionPointer(signed int, ScreenFade2, (char a1, unsigned __int8 a2, unsigned __int8 a3, unsigned __int8 a4), 0x478790);
FastcallFunctionPointer(void, AL_OdeMenuSetMode, (int t, int mode), 0x57E6D0);
FunctionPointer(ODE_MENU_STATE, AL_OdeMenuGetState, (), 0x57E710);
VoidFunc(AL_OdeMenuSetEnd, 0x57E6F0);
FunctionPointer(void, LargeTitleBarExecutor_Load, (int a1, float a2, float a3), 0x5ABD30);
FunctionPointer(int, AL_Change_Chao_Area, (int a1), 0x52B5B0);

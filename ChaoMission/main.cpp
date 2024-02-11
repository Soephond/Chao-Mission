#include "pch.h"

#include <fstream>
#include <iostream>
#include <array>
#include "Windows.h"

#include "FunctionHook.h"

using namespace std;

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

FastcallFunctionHook<void, ODE_MENU_MASTER_WORK *> Call_Goodbye_Menu_hk(0x5A6F50);

DataPointer(int, dword_1A557C8, 0x1A557C8);
DataArray(Al_MSGWarnKinderMessageThing, Al_MSGWarnKinderMessageArray, 0x1A259E8, 4);
DataPointer(AL_MSGWarnData, SomeAL_MSGWarn, 0x1A276D8);
DataPointer(ODE_MENU_MASTER_WORK*, AL_OdekakeMenuMaster_Data_ptr, 0x1AED324);

FunctionPointer(void, LargeTitleBarExecutor_Load, (int a1, float a2, float a3), 0x5ABD30);
FunctionPointer(void, sub_5ACBB0, (float a1, float a2), 0x5ACBB0);
FastcallFunctionPointer(void, AL_OdeMenuSetMode, (int t, int mode), 0x57E6D0);
FunctionPointer(ODE_MENU_STATE, AL_OdeMenuGetState, (), 0x57E710);
VoidFunc(AL_OdeMenuSetEnd, 0x57E6F0);
FastcallFunctionPointer(void, sub_443030, (int a1), 0x443030);
FunctionPointer(void, AlMsgWarnCreate, (float a1, float a2, float a3, float a4), 0x5436F0);
VoidFunc(AlMsgWarnOpen, 0x543800);
FastcallFunctionPointer(void, AlMsgWarnAddLineC, (int a1), 0x52DC90);
FunctionPointer(Message380 *, sub_543860, (), 0x543860);
FunctionPointer(void, sub_57EE80, (float a1, float a2, float a3), 0x57EE80);
FunctionPointer(KinderCoMessageThing *, sub_543830, (), 0x543830);
VoidFunc(sub_5437B0, 0x5437B0);
VoidFunc(sub_52FB80, 0x52FB80);
FunctionPointer(void, AlMsgWinAddLineC, (KinderCoMessageThing* a2, const char* a1, bool a3), 0x579E30);
FastcallFunctionPointer(ChaoData*, AL_GBAManagerGetChaoData, (), 0x532D00);
FunctionPointer(int, sub_52B5B0, (int a1), 0x52B5B0);
FunctionPointer(ODE_MENU_MASTER_WORK *, AL_OdeMenuChangeStage, (), 0x57E680);
FastcallFunctionPointer(signed int, ScreenFade2, (char a1, unsigned __int8 a2, unsigned __int8 a3, unsigned __int8 a4), 0x478790);

bool FirstGoodbye = true;

extern "C" {

    void __fastcall Call_Goodbye_Menu_sub_5A6F50_(ODE_MENU_MASTER_WORK *OdeMenuMasterWork)
    {
        /*if (FirstGoodbye)
        {
            Call_Goodbye_Menu_hk.Original(OdeMenuMasterWork);
			FirstGoodbye = false;
		}*/
        

        ODE_MENU_MASTER_WORK* v1; // edi
        Uint32 press; // eax
        const char* v3; // edi
        ChaoData* DataOfChao; // eax
        AL_GBAManagerExecutor_Data* v5; // eax
        int timer; // eax
        int v7; // eax
        int v8; // eax

        v1 = AL_OdekakeMenuMaster_Data_ptr;
        if (AL_OdekakeMenuMaster_Data_ptr)
        {
            switch (AL_OdekakeMenuMaster_Data_ptr->mode)
            {
            case 0:
                LargeTitleBarExecutor_Load(AL_OdekakeMenuMaster_Data_ptr->CurrStage, 650.0, 66.0);
                AlMsgWarnCreate(80.0, 120.0, 480.0, 96.0);
                AlMsgWarnOpen();
                AlMsgWarnAddLineC(45);
                //AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, "Hallo", TextLanguage == 0);
                sub_543860();
                //ChaoParamWindowExecutor_Load(160.0, 248.0);
                //sub_5ACBB0(480.0, 248.0);
                AL_OdeMenuSetMode(0, 1);
                PlayMusic("chao_g_iede.adx");
                ResetMusic();
                goto LABEL_4;
            case 1:
            LABEL_4:
                if (AL_OdeMenuGetState() == ODE_MENU_STATE_OK)
                {
                    goto LABEL_5;
                }
                goto LABEL_51;
            case 2:
                if ((MenuButtons_Pressed[0] & 0x10) != 0)
                {
                    --v1->cursorY;
                    PlaySoundProbably(0x8000, 0, 0, 0);
                }
                if ((MenuButtons_Pressed[0] & 0x20) != 0)
                {
                    ++v1->cursorY;
                    PlaySoundProbably(0x8000, 0, 0, 0);
                }
                if (v1->cursorY < 0)
                {
                    v1->cursorY = 0;
                }
                if (v1->cursorY > 1)
                {
                    v1->cursorY = 1;
                }
                press = ControllerPointers[0]->press;
                if ((press & 4) != 0)
                {
                    if (v1->cursorY != 1)
                    {
                        AL_OdeMenuSetMode(0, 3);
                        PlaySoundProbably(4110, 0, 0, 0);
                        dword_1A557C8 = 0;
                        return;
                    }
                    AL_OdeMenuSetEnd();
                    AL_OdeMenuSetMode(0, 14);
                    v3 = "chao_r_e.ADX";
                    goto LABEL_20;
                }
                if ((press & 2) != 0)
                {
                    AL_OdeMenuSetEnd();
                    AL_OdeMenuSetMode(0, 14);
                    sub_443030(0);
                    v3 = "chao_r_e.ADX";
                LABEL_20:
                    PlayMusic(v3);
                    ResetMusic();
                    PlaySoundProbably(32787, 0, 0, 0);
                    dword_1A557C8 = 0;
                    return;
                }
            LABEL_51:
                dword_1A557C8 = 0;
                break;
            case 3:
                AlMsgWarnCreate(80.0, 120.0, 480.0, 96.0);
                AlMsgWarnOpen();
                AlMsgWarnAddLineC(40);
                sub_543860();
                AlMsgWarnAddLineC(41);
                AL_OdeMenuSetMode(0, 4);
                dword_1A557C8 = 0;
                return;
            case 4:
                if (Al_MSGWarnKinderMessageArray[0].pkindercomessagething14->wordsLeftMaybe > 0)
                {
                    goto LABEL_51;
                }
                sub_57EE80(80.0, 240.0, 120.0);
                SomeAL_MSGWarn.field_3 = 1;
                AL_OdeMenuSetMode(0, 5);
                dword_1A557C8 = 0;
                return;
            case 5:
                if (SomeAL_MSGWarn.field_0)
                {
                    goto LABEL_51;
                }
                if (SomeAL_MSGWarn.field_3)
                {
                    goto LABEL_33;
                }
                sub_543830();
                AL_OdeMenuSetMode(0, 6);
                SomeAL_MSGWarn.field_0 = 0;
                dword_1A557C8 = 0;
                return;
            case 6:
                AlMsgWarnAddLineC(42);
                sub_543860();
                AlMsgWarnAddLineC(43);
                AL_OdeMenuSetMode(0, 7);
                dword_1A557C8 = 0;
                return;
            case 7:
                if (Al_MSGWarnKinderMessageArray[0].pkindercomessagething14->wordsLeftMaybe > 0)
                {
                    goto LABEL_51;
                }
                sub_57EE80(80.0, 240.0, 120.0);
                SomeAL_MSGWarn.field_3 = 1;
                AL_OdeMenuSetMode(0, 8);
                dword_1A557C8 = 0;
                return;
            case 8:
                if (SomeAL_MSGWarn.field_0)
                {
                    goto LABEL_51;
                }
                if (SomeAL_MSGWarn.field_3)
                {
                LABEL_33:
                    sub_5437B0();
                    AL_OdeMenuSetMode(0, 2);
                    SomeAL_MSGWarn.field_0 = 0;
                    dword_1A557C8 = 0;
                }
                else
                {
                    sub_543830();
                    AL_OdeMenuSetMode(0, 9);
                    SomeAL_MSGWarn.field_0 = 0;
                    dword_1A557C8 = 0;
                }
                return;
            case 9:
                AlMsgWarnAddLineC(44);
                sub_543860();
                AlMsgWarnAddLineC(45);
                sub_543860();
                AlMsgWarnAddLineC(46);
                sub_543860();
                AlMsgWarnAddLineC(47);
                sub_543860();
                AlMsgWarnAddLineC(48);
                sub_52FB80();
                AL_OdeMenuSetMode(0, 10);
                dword_1A557C8 = 0;
                return;
            case 0xA:
                if (Al_MSGWarnKinderMessageArray[0].pkindercomessagething14->wordsLeftMaybe > 0)
                {
                    goto LABEL_51;
                }
                sub_5437B0();
                DataOfChao = AL_GBAManagerGetChaoData();
                if (DataOfChao)
                {
                    memset(DataOfChao, 0, sizeof(ChaoData));
                    v5 = AL_GBAManagerExecutor_ptr;
                    if (AL_GBAManagerExecutor_ptr)
                    {
                        AL_GBAManagerExecutor_ptr->insertedChao = 0;
                        v5->ChaoDataPointer = 0;
                    }
                }
                sub_52B5B0(LastChaoArea);
                AL_OdeMenuSetMode(0, 18);
                dword_1A557C8 = 0;
                return;
            case 0xB:
                goto LABEL_43;
            case 0xC:
                timer = AL_OdekakeMenuMaster_Data_ptr->timer;
                AL_OdekakeMenuMaster_Data_ptr->timer = timer + 1;
                if (timer <= 60)
                {
                    goto LABEL_51;
                }
            LABEL_5:
                AL_OdeMenuSetMode(0, 2);
                dword_1A557C8 = 0;
                return;
            case 0xD:
                v7 = AL_OdekakeMenuMaster_Data_ptr->timer;
                AL_OdekakeMenuMaster_Data_ptr->timer = v7 + 1;
                if (v7 <= 60)
                {
                    goto LABEL_51;
                }
            LABEL_43:
                AL_OdeMenuSetMode(0, 14);
                dword_1A557C8 = 0;
                return;
            case 0xE:
                v8 = AL_OdekakeMenuMaster_Data_ptr->timer;
                AL_OdekakeMenuMaster_Data_ptr->timer = v8 + 1;
                if (v8 <= 30)
                {
                    goto LABEL_51;
                }
                AL_OdeMenuSetMode(0, 15);
                dword_1A557C8 = 0;
                return;
            case 0xF:
                if (AL_OdeMenuGetState())
                {
                    goto LABEL_51;
                }
                AL_OdeMenuSetMode(0, 16);
                dword_1A557C8 = 0;
                return;
            case 0x10:
                AL_OdekakeMenuMaster_Data_ptr->NextStage = 0;
                AL_OdeMenuChangeStage();
                dword_1A557C8 = 0;
                return;
            case 0x11:
                if (ScreenFade2(255, 255u, 255u, 255u))
                {
                    sub_52B5B0(LastChaoArea);
                }
                goto LABEL_51;
            default:
                goto LABEL_51;
            }
        }
    }

	

	// Optional.
	// This function runs code one time when the game starts up. Great for loading assets and setting things up.
	__declspec(dllexport) void __cdecl Init(const char* path, const HelperFunctions& helperFunctions)
	{
        Call_Goodbye_Menu_hk.Hook(Call_Goodbye_Menu_sub_5A6F50_);
	}

	// Optional.
	// This function runs code on every frame of the game, INCLUDING being in menus, cutscenes, etc.
	// It is recommended to test for game state so that you only run code at a specific time.
	__declspec(dllexport) void __cdecl OnFrame()
	{
        if (AL_OdekakeMenuMaster_Data_ptr)
        {
            if (AL_OdekakeMenuMaster_Data_ptr->CurrStage != 5 && !FirstGoodbye)
            {
                FirstGoodbye = true;
            }
        }
	}

	// Optional.
	// This function runs code every time the player inputs. Good for adding custom inputs / overriding events.
	__declspec(dllexport) void __cdecl OnInput()
	{
	}

	// Optional.
	// This function runs while the game processes input.
	__declspec(dllexport) void __cdecl OnControl()
	{
	}

	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer }; // This is needed for the Mod Loader to recognize the DLL.
}

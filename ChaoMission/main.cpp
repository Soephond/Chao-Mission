#include "pch.h"

#include <fstream>
#include <iostream>
#include <array>
#include <format>
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

#pragma pack(push, 8)
struct __declspec(align(4)) BlackMarketItemAttributes
{
	int PurchasePrice;
	int SalePrice;
	__int16 RequiredEmblems;
	__int16 Name;
	__int16 Descriptions;
	__int16 Unknown;
};
#pragma pack(pop)


struct BlackMarketCategoryAttribute
{
	BlackMarketItemAttributes* attrib;
	int Count;
};



char ChaoNameDecoder[256]
{
	'\n',
	'!',
	'\"',
	'#',
	'$',
	'%',
	'&',
	'\'',
	'(',
	')',
	'*',
	'+',
	',',
	'-',
	'.',
	'/',
	'0',
	'1',
	'2',
	'3',
	'4',
	'5',
	'6',
	'7',
	'8',
	'9',
	':',
	';',
	'<',
	'=',
	'>',
	'?',
	'@',
	'A',
	'B',
	'C',
	'D',
	'E',
	'F',
	'G',
	'H',
	'I',
	'J',
	'K',
	'L',
	'M',
	'N',
	'O',
	'P',
	'Q',
	'R',
	'S',
	'T',
	'U',
	'V',
	'W',
	'X',
	'Y',
	'Z',
	'[',
	'¥',
	']',
	'^',
	'_',
	'‘',
	'a',
	'b',
	'c',
	'd',
	'e',
	'f',
	'g',
	'h',
	'i',
	'j',
	'k',
	'l',
	'm',
	'n',
	'o',
	'p',
	'q',
	'r',
	's',
	't',
	'u',
	'v',
	'w',
	'x',
	'y',
	'z',
	'{',
	'|',
	'}',
	'~',
	' ',
	'À',
	'Á',
	'Â',
	'Ã',
	'Ä',
	'Å',
	'Æ',
	'Ç',
	'È',
	'É',
	'Ê',
	'Ë',
	'Ì',
	'Í',
	'Î',
	'Ï',
	'Ð',
	'Ñ',
	'Ò',
	'Ó',
	'Ô',
	'Õ',
	'Ö',
	'¿',
	'Ø',
	'Ù',
	'Ú',
	'Û',
	'Ü',
	'Ý',
	'Þ',
	'ß',
	'à',
	'á',
	'â',
	'ã',
	'ä',
	'å',
	'æ',
	'ç',
	'è',
	'é',
	'ê',
	'ë',
	'ì',
	'í',
	'î',
	'ï',
	'ð',
	'ñ',
	'ò',
	'ó',
	'ô',
	'õ',
	'ö',
	'¡',
	'ø',
	'ù',
	'ú',
	'û',
	'ü',
	'ý',
	'þ',
	'ÿ',
	'ァ',
	'ア',
	'ィ',
	'イ',
	'ゥ',
	'ウ',
	'ェ',
	'エ',
	'ォ',
	'オ',
	'カ',
	'ガ',
	'キ',
	'ギ',
	'ク',
	'グ',
	'ケ',
	'ゲ',
	'コ',
	'ゴ',
	'サ',
	'ザ',
	'シ',
	'ジ',
	'ス',
	'ズ',
	'セ',
	'ゼ',
	'ソ',
	'ゾ',
	'タ',
	'ダ',
	'チ',
	'ヂ',
	'ツ',
	'ッ',
	'ヅ',
	'テ',
	'デ',
	'ト',
	'ド',
	'ナ',
	'ニ',
	'ヌ',
	'ネ',
	'ノ',
	'ハ',
	'バ',
	'パ',
	'ヒ',
	'ビ',
	'ピ',
	'フ',
	'ブ',
	'プ',
	'ヘ',
	'ベ',
	'ベ',
	'ホ',
	'ボ',
	'ポ',
	'マ',
	'ミ',
	'ム',
	'メ',
	'モ',
	'ャ',
	'ヤ',
	'ュ',
	'ユ',
	'ョ',
	'ヨ',
	'ラ',
	'リ',
	'ル',
	'レ',
	'ロ',
	'ヮ',
	'ワ',
	'ﾞ',
	'ﾟ',
	'ヲ',
	'ン',
	'。',
	'、',
	'〒',
	'・',
	'★',
	'♀',
	'♂',
	'♪',
	'…',
	'「',
	'」',
	'ヴ',
	' '
};


char* DecodeChaoName(char name[])
{
	int i = 0;
	char result[7];
	while (name[i] != 0 && i < 7)
	{
		if (name[i] < 256) result[i] = ChaoNameDecoder[name[i]];
		else result[i] = '_';
		i++;
	}
	return result;
}

FastcallFunctionHook<void, ODE_MENU_MASTER_WORK*> Call_Goodbye_Menu_hk(0x5A6F50);

DataPointer(int, dword_1A557C8, 0x1A557C8);
DataArray(Al_MSGWarnKinderMessageThing, Al_MSGWarnKinderMessageArray, 0x1A259E8, 4);
DataArray(BlackMarketCategoryAttribute, CategoryAttribs, 0x8A8728, 9);
DataPointer(AL_MSGWarnData, SomeAL_MSGWarn, 0x1A276D8);
DataPointer(ODE_MENU_MASTER_WORK*, AL_OdekakeMenuMaster_Data_ptr, 0x1AED324);
DataPointer(int, dword_1A267D0, 0x1A267D0);
DataPointer(RingDisplayData, RingDisplayData_, 0x1A27870);

FunctionPointer(void, LoadRingDisplay, (float a1, float a2), 0x5A6AC0);
VoidFunc(DeleteRingDisplay, 0x5A6B10);
//FastcallFunctionPointer(void, RingDisplay_AddRings, (int a1), 0x5A6B30);
FunctionPointer(void, LargeTitleBarExecutor_Load, (int a1, float a2, float a3), 0x5ABD30);
FunctionPointer(void, sub_5ACBB0, (float a1, float a2), 0x5ACBB0);
FastcallFunctionPointer(void, AL_OdeMenuSetMode, (int t, int mode), 0x57E6D0);
FunctionPointer(ODE_MENU_STATE, AL_OdeMenuGetState, (), 0x57E710);
VoidFunc(AL_OdeMenuSetEnd, 0x57E6F0);
FastcallFunctionPointer(void, sub_443030, (int a1), 0x443030);
FunctionPointer(void, AlMsgWarnCreate, (float a1, float a2, float a3, float a4), 0x5436F0);
VoidFunc(AlMsgWarnOpen, 0x543800);
FastcallFunctionPointer(void, AlMsgWarnAddLineC, (int a1), 0x52DC90);
FunctionPointer(Message380*, sub_543860, (), 0x543860);
FunctionPointer(void, AL_CreateChoose, (float a1, float a2, float a3), 0x57EE80);
FunctionPointer(KinderCoMessageThing*, AlMsgWarnClear, (), 0x543830);
VoidFunc(AlMsgWarnRemove, 0x5437B0);
VoidFunc(sub_52FB80, 0x52FB80);
//FunctionPointer(void, AlMsgWinAddLineC, (KinderCoMessageThing* a2, const char* a1, bool a3), 0x579E30);
FastcallFunctionPointer(ChaoData*, AL_GBAManagerGetChaoData, (), 0x532D00);
FunctionPointer(int, sub_52B5B0, (int a1), 0x52B5B0);
FunctionPointer(ODE_MENU_MASTER_WORK*, AL_OdeMenuChangeStage, (), 0x57E680);
FastcallFunctionPointer(signed int, ScreenFade2, (char a1, unsigned __int8 a2, unsigned __int8 a3, unsigned __int8 a4), 0x478790);

bool FirstGoodbye = true;

extern "C" {

	//void __usercall ALMsgWinAddLineC(KinderCoMessageThing* a2@<esi>, const char* a1@<ebx>, bool a3)
	const int sub_579E30Ptr = 0x579E30;
	void AlMsgWinAddLineC(KinderCoMessageThing* a2, const char* a1, bool a3)
	{
		dword_1A267D0 = -1;
		__asm
		{
			push[a3]
			mov ebx, [a1]
			mov esi, [a2]
			call sub_579E30Ptr
			add esp, 4
		}
	}

	//void __usercall RingDisplay_AddRings(signed int a1@<eax>)
	const int sub_5A6B30 = 0x5A6B30;
	void RingDisplay_AddRings(signed int a1)
	{
		__asm
		{
			mov eax, [a1]
			call sub_5A6B30
		}
	}

	int CalculateReward(ChaoData* chao)
	{
		ChaoDataBase* chaoInfo = &chao->data;
		int eggSalePrice = CategoryAttribs[9].attrib[chaoInfo->EggColor + 0x10].SalePrice;
		float specialChaoMult = chaoInfo->Type > 0x13 && chaoInfo->Reincarnations >= 2 ? 1.5f : 1;
		float randomMult = ((rand() % 20) + 90) / 100.0f; //Get a random number between 90 and 110 and divide by 100 to get a float between 0.9 and 1.1
		int happinessMult = (chaoInfo->Happiness / 600) + 1;
		int ageExtra = ((1000 * chaoInfo->Lifespan) / chaoInfo->Lifespan2) + (chaoInfo->Reincarnations * sqrt(0.87) * 800);
		float gradeSqrt = 0.0f;
		int totalLevels = 0;
		int levelExtra = 0;
		int total = 0;

		for (int i = 0; i < 5; i++)
		{
			gradeSqrt *= sqrt((chaoInfo->StatGrades[i] * 0.48f) + 0.6f);
			totalLevels += chaoInfo->StatLevels[i];
		}

		levelExtra = gradeSqrt * totalLevels * 25;

		total = (eggSalePrice + ((levelExtra + ageExtra) * specialChaoMult )) * randomMult * happinessMult;

		if (chaoInfo->Reincarnations == 0 && chaoInfo->Type == ChaoType_Child)
		{
			total *= .10;
		}

		return total;
	}

	void __fastcall Call_Goodbye_Menu_sub_5A6F50_(ODE_MENU_MASTER_WORK* OdeMenuMasterWork)
	{
		ODE_MENU_MASTER_WORK* v1; // edi
		Uint32 press; // eax
		const char* v3; // edi
		ChaoData* DataOfChao; // eax
		AL_GBAManagerExecutor_Data* v5; // eax
		int timer; // eax
		int v7; // eax
		int v8; // eax
		int reward = 100;
		std::string rewardString;

		v1 = AL_OdekakeMenuMaster_Data_ptr;
		if (AL_OdekakeMenuMaster_Data_ptr)
		{
			switch (AL_OdekakeMenuMaster_Data_ptr->mode)
			{
			case 0:
				LargeTitleBarExecutor_Load(AL_OdekakeMenuMaster_Data_ptr->CurrStage, 650.0, 66.0);
				AlMsgWarnCreate(307.5, 126.5, 300.0, 245.0);
				AlMsgWarnOpen();
				LoadRingDisplay(40, 400);
				AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, "Hello, Welcome to C.H.A.O.:", TextLanguage == 0);
				AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, "Chao.", TextLanguage == 0);
				AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, "Home.", TextLanguage == 0);
				AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, "Adoption.", TextLanguage == 0);
				AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, "Organization.", TextLanguage == 0);
				sub_543860();
				AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, "This is program is a initiative to send Chao to a new home where they can be happy.", TextLanguage == 0);
				sub_543860();
				AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, "Once your Chao is accepted into the program, you won't be able reverse this.", TextLanguage == 0);
				sub_52FB80();
				ChaoParamWindowExecutor_Load(160.0, 248.0);
				AL_OdeMenuSetMode(0, 1);
				PlayMusic("chao_g_iede.adx");
				ResetMusic();
				goto LABEL_4;
			case 1:
			LABEL_4:
				if (AL_OdeMenuGetState() == ODE_MENU_STATE_OK)
				{
				LABEL_5:
					AL_OdeMenuSetMode(0, 2);
					dword_1A557C8 = 0;
					return;
				}
				goto LABEL_51;
			case 2:
				if (Al_MSGWarnKinderMessageArray[0].pkindercomessagething14->wordsLeftMaybe > 0)
				{
					goto LABEL_51;
				}
				AL_OdeMenuSetMode(0, 3);
			LABEL_51:
				dword_1A557C8 = 0;
				break;
			case 3:
				AlMsgWarnRemove();
				AlMsgWarnCreate(307.5, 126.5, 300.0, 120.0);
				AlMsgWarnOpen();
				AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, "Do you want to send your chao away for adoption?", TextLanguage == 0);
				AL_OdeMenuSetMode(0, 4);
			case 4:
				if (Al_MSGWarnKinderMessageArray[0].pkindercomessagething14->wordsLeftMaybe > 0)
				{
					goto LABEL_51;
				}
				AL_CreateChoose(308.0, 260.0, 120.0);
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
				AlMsgWarnClear();
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
				AL_CreateChoose(308.0, 260.0, 120.0);
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
					AL_OdeMenuSetEnd();
					AlMsgWarnRemove();
					AL_OdeMenuSetMode(0, 15);
					SomeAL_MSGWarn.field_0 = 0;
					dword_1A557C8 = 0;
				}
				else
				{
					AlMsgWarnClear();
					AL_OdeMenuSetMode(0, 9);
					SomeAL_MSGWarn.field_0 = 0;
					dword_1A557C8 = 0;
				}
				return;
			case 9:
				AlMsgWarnRemove();
				AlMsgWarnCreate(307.5, 126.5, 300.0, 245.0);
				AlMsgWarnOpen();
				AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, "CALCULATING REWARD", TextLanguage == 0);
				sub_543860();
				AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, "Your reward for sending this chao to the adoption program has been decided.", TextLanguage == 0);
				sub_52FB80();
				AL_OdeMenuSetMode(0, 10);
				dword_1A557C8 = 0;
				return;
			case 0xA:
				if (Al_MSGWarnKinderMessageArray[0].pkindercomessagething14->wordsLeftMaybe > 0)
				{
					goto LABEL_51;
				}
				DataOfChao = AL_GBAManagerGetChaoData();
				if (DataOfChao)
				{
					reward = CalculateReward(DataOfChao);
				}
				AlMsgWarnClear();
				rewardString = "You will receive " + to_string(reward) + " rings";
				AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, rewardString.c_str(), TextLanguage == 0);
				sub_52FB80();
				RingDisplay_AddRings(RingDisplayData_.field_C + reward);
				AL_OdeMenuSetMode(0, 11);
			case 0xB:
				if (Al_MSGWarnKinderMessageArray[0].pkindercomessagething14->wordsLeftMaybe > 0)
				{
					goto LABEL_51;
				}
				AlMsgWarnClear();
				AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, "Your Chao will be send to a home that has a lot of love for them.", TextLanguage == 0);
				sub_543860();
				AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, "They will always remember you.", TextLanguage == 0);
				sub_543860();
				AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, "They wave goodbye one last time with a smile on their face.", TextLanguage == 0);
				sub_52FB80();
				AL_OdeMenuSetMode(0, 12);
			case 0xC:
				if (Al_MSGWarnKinderMessageArray[0].pkindercomessagething14->wordsLeftMaybe > 0)
				{
					goto LABEL_51;
				}
				AlMsgWarnRemove();
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
				AL_OdeMenuSetMode(0, 19);
				dword_1A557C8 = 0;
				return;

			case 0xD:
				AL_OdeMenuSetMode(0, 15);
				dword_1A557C8 = 0;
				return;
			case 0xF:
				v8 = AL_OdekakeMenuMaster_Data_ptr->timer;
				AL_OdekakeMenuMaster_Data_ptr->timer = v8 + 1;
				if (v8 <= 30)
				{
					goto LABEL_51;
				}
				AL_OdeMenuSetMode(0, 16);
				dword_1A557C8 = 0;
				return;
			case 0x10:
				if (AL_OdeMenuGetState())
				{
					goto LABEL_51;
				}
				AL_OdeMenuSetMode(0, 17);
				dword_1A557C8 = 0;
				return;
			case 0x11:
				v3 = "chao_r_e.ADX";
				PlayMusic(v3);
				ResetMusic();
				PlaySoundProbably(32787, 0, 0, 0);
				AL_OdekakeMenuMaster_Data_ptr->NextStage = 0;
				AL_OdeMenuChangeStage();
				AlMsgWarnRemove();
				DeleteRingDisplay();
				dword_1A557C8 = 0;
				return;
			case 0x12:
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

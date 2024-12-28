#include <Windows.h>

#include "al_chao_home.h"

using namespace std;

NJS_TEXNAME AL_ODE_CHAO_TEXNAME[1];
NJS_TEXLIST AL_ODE_CHAO_TEXLIST[] = { AL_ODE_CHAO_TEXNAME, 1 };

int CalculateReward(CHAO_PARAM_GC* chao)
{
	int eggSalePrice = CategoryAttribs[9].attrib[(Uint8)chao->body.EggColor + 0x10].SalePrice / 2;
	float specialChaoMult = chao->Type >= (eCHAO_TYPE)0x13 && chao->nbSucceed >= 2 ? 2.0f : 1.0f;
	float randomMult = ((rand() % 20) + 90) / 100.0f; //Get a random number between 90 and 110 and divide by 100 to get a float between 0.9 and 1.1
	float happinessMult = (static_cast<float>(chao->like) / 600) + 1;
	int ageExtra = chao->nbSucceed * static_cast<int>(sqrt(0.87)) * 800;
	float gradeSqrt = 1.0f;
	int totalLevels = 0;
	int levelExtra = 0;
	int total = 0;

	for (int i = 0; i < 5; i++)
	{
		gradeSqrt *= (chao->Abl[i] * 0.048f) + 0.6f;
		totalLevels += chao->Lev[i];
	}

	if (totalLevels == 0)
	{
		totalLevels = 1;
	}

	if (ageExtra == 0)
	{
		ageExtra = 1;
	}

	levelExtra = (int)gradeSqrt * totalLevels * 35;

	total = (int)(((float)eggSalePrice + ((float)levelExtra + (float)ageExtra) * specialChaoMult) * (randomMult * happinessMult));

	if (chao->nbSucceed == 0 && chao->Type == eCHAO_TYPE::Child)
	{
		total =  (int)((float)total * .06);
	}

	return total;
}

NJS_TEXLIST* GetCHAO_TEXLIST()
{
	return AL_ODE_CHAO_TEXLIST;
}

void __fastcall Handle_CHAO_Menu(ODE_MENU_MASTER_WORK* OdeMenuMasterWork)
{
	Handle_Adoption_Screen(OdeMenuMasterWork);
}

void __fastcall Handle_Adoption_Screen(ODE_MENU_MASTER_WORK* OdeMenuMasterWork)
{
	ODE_MENU_MASTER_WORK* v1; // edi
	//Uint32 press; // eax
	const char* v3; // edi
	CHAO_PARAM_GC* DataOfChao; // eax
	AL_GBAManagerExecutor_Data* v5; // eax
	//int timer; // eax
	//int v7; // eax
	int v8; // eax
	int reward = 100;
	std::string rewardString;
	float x = 1.0f;

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
			AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, "This is program is an initiative to send Chao to a new home where they can be happy.", TextLanguage == 0);
			sub_543860();
			AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, "Once your Chao is accepted into the program, you won't be able reverse this.", TextLanguage == 0);
			sub_52FB80();
			ChaoParamWindowExecutor_Load(160.0, 248.0);
			AL_OdeMenuSetMode(0, 1);
			PlayMusic("chao_g_iede.adx");
			ResetMusic();
			goto LABEL_51;
		case 1:
			if (AL_OdeMenuGetState() == ODE_MENU_STATE_OK)
			{
			//LABEL_5:
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
			DataOfChao = AL_GBAManagerGetChaoData();
			if (DataOfChao->like >= 85)
			{
				AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, "The chao leaps in your arms to say good-bye.", TextLanguage == 0);
				sub_543860();
				AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, "They cry in your arms but you can also make out a big smile that they have a new wonderful life ahead of them.", TextLanguage == 0);
				sub_543860();
				AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, "While they are walking away they keep on looking back, smiling and waving.", TextLanguage == 0);
			}
			else if (DataOfChao->like <= -50)
			{
				AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, "Your chao cries when they hear they go to a place where they are finally loved.", TextLanguage == 0);
				sub_543860();
				AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, "They don't look back.", TextLanguage == 0);
				sub_543860();
				AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, "---------------------", TextLanguage == 0);
				AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, "YOU GOT A MESSAGE FROM C.H.A.O.:", TextLanguage == 0);
				sub_543860();
				AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, "'Please raise your chao's to be happier!", TextLanguage == 0);
				AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, "I mean, we didn't expect much, but come on!'", TextLanguage == 0);
			}
			else if (DataOfChao->like > -50 && DataOfChao->like < 0)
			{
				AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, "The chao walks away to their new home.", TextLanguage == 0);
				sub_543860();
				AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, "The air is filled with a sense of contempt.", TextLanguage == 0);
				sub_543860();
				AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, "They look back one last time and you see an expression of relief on their face.", TextLanguage == 0);
			}
			else
			{
				AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, "Your Chao will be send to a home that has a lot of love for them.", TextLanguage == 0);
				sub_543860();
				AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, "They will always remember you.", TextLanguage == 0);
				sub_543860();
				AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, "They wave goodbye one last time with a smile on their face.", TextLanguage == 0);
			}

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
				memset(DataOfChao, 0, sizeof(CHAO_PARAM_GC));
				v5 = AL_GBAManagerExecutor_ptr;
				if (AL_GBAManagerExecutor_ptr)
				{
					AL_GBAManagerExecutor_ptr->insertedChao = 0;
					v5->ChaoDataPointer = 0;
				}
			}
			AL_Change_Chao_Area(LastChaoArea);
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
			if (ScreenFade2((char)255, 255u, 255u, 255u))
			{
				AL_Change_Chao_Area(LastChaoArea);
			}
			goto LABEL_51;
		default:
			goto LABEL_51;
		}
	}
}
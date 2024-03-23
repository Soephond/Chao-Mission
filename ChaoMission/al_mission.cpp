#include "al_mission.h"
#include "chao_data.h"

ChaoHudThingB MissionMenu[] = {
		{1, 128, 32, 22.0f / 1024.0f, 297.0f / 1024.0f, 320.0f / 1024.0f, 364.0f / 1024.0f, AL_ODE_CHAO_TEXLIST, 0}, //text
		{1, 128, 32, 22.0f / 1024.0f, 297.0f / 1024.0f, 320.0f / 1024.0f, 364.0f / 1024.0f, AL_ODE_CHAO_TEXLIST, 0}, //grey text
		{1, 40, 40, 410.0f / 1024.0f, 36.0f / 1024.0f, 562.0f / 1024.0f, 188.0f / 1024.0f, AL_ODE_CHAO_TEXLIST, 0}, //icon
		{1, 50, 50, 610.0f / 1024.0f, 30.0f / 1024.0f, 751.0f / 1024.0f, 193.0f / 1024.0f, AL_ODE_CHAO_TEXLIST, 0}, //icon black and white
		{1, 185, 46, 22.0f / 1024.0f, 297.0f / 1024.0f, 320.0f / 1024.0f, 364.0f / 1024.0f, AL_ODE_CHAO_TEXLIST, 0}, //text titlebar
		{1, 52.5, 90, 73.0f / 1024.0f, 756.0f / 1024.0f, 173.0f / 1024.0f, 940.0f / 1024.0f, AL_ODE_CHAO_TEXLIST, 0}, //left Arrow
		{1, 52.5, 90, 215.0f / 1024.0f, 756.0f / 1024.0f, 315.0f / 1024.0f, 940.0f / 1024.0f, AL_ODE_CHAO_TEXLIST, 0}, //right Arrow
		{1, 52.5, 90, 336.0f / 1024.0f, 756.0f / 1024.0f, 436.0f / 1024.0f, 940.0f / 1024.0f, AL_ODE_CHAO_TEXLIST, 0}, //left Arrow inactive
		{1, 52.5, 90, 467.0f / 1024.0f, 756.0f / 1024.0f, 577.0f / 1024.0f, 940.0f / 1024.0f, AL_ODE_CHAO_TEXLIST, 0}, //right Arrow inactive
};

const char* Checkmark = "Cleared";
const char* Denymark = "Not Cleared";

int CurrentMission = 0;
int MissionCount = Missions.Amount;

template <typename T>
int GetFlagTotal(T flag)
{
	int count = 0;
	for (int i = 0; i < sizeof(T); i++)
	{
		if (flag & (1 << i)) count++;
	}
	return count;
}

template<typename T>
bool Check_Value(ValueCheckStruct<T> checkValue, T value)
{
	bool result;

	switch (checkValue.range)
	{
	case RangeType_MinRange:
		result = value >= checkValue.min;
		break;

	case RangeType_MaxRange:
		result = value <= checkValue.max;
		break;

	case RangeType_Range:
		result = value >= checkValue.min && value <= checkValue.max;
		break;

	case RangeType_NoRange:
		result = value == checkValue.value;
		break;
	}

	return checkValue.inverted ? !result : result;
}

bool Handle_ChaoType_Check(CHAO_PARAM_GC* chao, decltype(ValueCheck::ChaoTypeCheck)* info)
{
	switch (info->Type)
	{
	case ChaoTypeCheck:
		return Check_Value(info->Check.Type, (ChaoType)chao->Type);

	case SwimFlyInfluenceCheck:
		return Check_Value(info->Check.SwimFlyInfluence, chao->body.VPos);

	case RunPowerInfluenceCheck:
		return Check_Value(info->Check.RunPowerInfluence, chao->body.HPos);

	case AlignmentCheck:
		return Check_Value(info->Check.Alignment, chao->body.AimAPos);

	case MagnitudeCheck:
		return Check_Value(info->Check.Magnitude, chao->body.growth);
	}

	return false;
}

bool Handle_Stat_Check(CHAO_PARAM_GC* chao, decltype(ValueCheck::StatCheck)* info)
{
	int skill = info->Skill;

	switch (info->Type)
	{
	case GradeCheck:
		return Check_Value(info->Check.Grade, (ChaoGrade)chao->Abl[skill]);

	case PointsCheck:
		return Check_Value(info->Check.Points, chao->Skill[skill]);

	case LevelCheck:
		return Check_Value(info->Check.Level, (byte)chao->Lev[skill]);

	case HappinessCheck:
		return Check_Value(info->Check.Happiness, chao->like);
	}

	return false;
}

bool Handle_Appearance_Check(CHAO_PARAM_GC* chao, decltype(ValueCheck::AppearanceCheck)* info)
{
	switch (info->Type)
	{
	case ColorCheck:
		return Check_Value(info->Check.Color, (ChaoColor)chao->body.ColorNum);

	case ToneCheck:
		return Check_Value(info->Check.Tone, (ChaoTone)chao->body.NonTex);

	case ShinyCheck:
		return Check_Value(info->Check.Shiny, (ChaoShiny)chao->body.MultiNum);

	case TextureCheck:
		return Check_Value(info->Check.Texture, (SA2BTexture)chao->body.JewelNum);

	case AnimalArmsCheck:
		return Check_Value(info->Check.AnimalArms, (Al_Animal)chao->PartsBTL.MinimalParts[AnimalParts_Arms]);

	case AnimalEarsCheck:
		return Check_Value(info->Check.AnimalEars, (Al_Animal)chao->PartsBTL.MinimalParts[AnimalParts_Ears]);

	case AnimalForeheadCheck:
		return Check_Value(info->Check.AnimalForehead, (Al_Animal)chao->PartsBTL.MinimalParts[AnimalParts_Forehead]);

	case AnimalHornCheck:
		return Check_Value(info->Check.AnimalHorn, (Al_Animal)chao->PartsBTL.MinimalParts[AnimalParts_Horn]);

	case AnimalLegsCheck:
		return Check_Value(info->Check.AnimalLegs, (Al_Animal)chao->PartsBTL.MinimalParts[AnimalParts_Legs]);

	case AnimalTailCheck:
		return Check_Value(info->Check.AnimalTail, (Al_Animal)chao->PartsBTL.MinimalParts[AnimalParts_Tail]);

	case AnimalWingsCheck:
		return Check_Value(info->Check.AnimalWings, (Al_Animal)chao->PartsBTL.MinimalParts[AnimalParts_Wings]);

	case AnimalFaceCheck:
		return Check_Value(info->Check.AnimalFace, (Al_Animal)chao->PartsBTL.MinimalParts[AnimalParts_Face]);
	}

	return false;
}

bool Handle_Ability_Check(CHAO_PARAM_GC* chao, decltype(ValueCheck::AbilityCheck)* info)
{
	short abilitiesLearned = 0;
	switch (info->Type)
	{
	case FlagCheck:
		abilitiesLearned = info->Check.AbilitiesLearned.value;
		return Check_Value(info->Check.AbilitiesLearned, (short)(chao->PartsBTL.MinimalFlag & abilitiesLearned));

	case TotalCheck:
		return Check_Value(info->Check.TotalAbilitiesLearned, GetFlagTotal(chao->PartsBTL.MinimalFlag));
	}

	return false;
}

//Handle SToyFlag
bool Handle_Toy_Check(CHAO_PARAM_GC* chao, decltype(ValueCheck::ToyCheck)* info)
{
	short toysOwned = 0;

	switch (info->Type)
	{
	case FlagCheck:
		toysOwned = info->Check.ToysUnlocked.value;
		return Check_Value(info->Check.ToysUnlocked, (short)(chao->knowledge.SToyFlag & toysOwned));

	case TotalCheck:
		return Check_Value(info->Check.TotalToysUnlocked, GetFlagTotal(chao->knowledge.SToyFlag));
	}

	return false;
}

bool Handle_Race_Check(CHAO_PARAM_GC* chao, decltype(ValueCheck::RaceCheck)* info)
{
	short medalsEarned = 0;

	switch (info->Type)
	{
	case FlagCheck:
		medalsEarned = info->Check.MedalsEarned.value;
		return Check_Value(info->Check.MedalsEarned, (MedalFlags)(chao->race.MedalFlag & medalsEarned));

	case TotalCheck:
		return Check_Value(info->Check.TotalMedals, (__int8)GetFlagTotal(chao->race.MedalFlag));
	}

	return false;
}

bool Handle_Karate_Check(CHAO_PARAM_GC* chao, decltype(ValueCheck::KarateCheck)* info)
{
	switch (info->Type)
	{
	case KarateRankCheck:
		return Check_Value(info->Check.Rank, (KarateRank)chao->karate.level);

	case AmountWonCheck:
		return Check_Value(info->Check.MatchesWon, chao->karate.nbWin);
	}

	return false;
}

bool Handle_Classes_Check(CHAO_PARAM_GC* chao, decltype(ValueCheck::ClassCheck)* info)
{
	int classes = chao->knowledge.ArtFlag | (chao->knowledge.DanceFlag << 8) | (chao->knowledge.SongFlag << 16) | (chao->knowledge.MusicFlag << 24);
	switch (info->Type)
	{
	case FlagCheck:
		return Check_Value(info->Check.ClassesAttended, classes);

	case TotalCheck:
		return Check_Value(info->Check.TotalClassesAttended, GetFlagTotal(classes));
	}

	return false;
}

bool Handle_Lifetime_Check(CHAO_PARAM_GC* chao, decltype(ValueCheck::LifetimeCheck)* info)
{
	switch (info->Type)
	{
	case AgeCheck:
		return Check_Value(info->Check.Age, (short)(chao->age / 60));

	case ReincarnationsCheck:
		return Check_Value(info->Check.Reincarnations, chao->nbSucceed);
	}

	return false;
}

bool Handle_Bond_Check(CHAO_PARAM_GC* chao, decltype(ValueCheck::BondCheck)* info)
{
	CharacterBondOrder character = info->Character;

	switch (info->Type)
	{
		case CharacterLikeCheck:
			return Check_Value(info->Check.Like, chao->knowledge.player[character].like);

		case CharacterFearCheck:
			return Check_Value(info->Check.Fear, chao->knowledge.player[character].fear);

		case CharacterDistanceCheck:
			return Check_Value(info->Check.Distance, chao->knowledge.player[character].distance);

		case CharacterMeetCheck:
			return Check_Value(info->Check.Meet, chao->knowledge.player[character].meet);
	}

	return false;
}

bool Handle_Check(CHAO_PARAM_GC* chao, ValueCheck* valueCheck, REQUIREMENT_TYPE type)
{
	switch (type)
	{
	case TypeRequirement:
		return Handle_ChaoType_Check(chao, &valueCheck->ChaoTypeCheck);

	case StatRequirement:
		return Handle_Stat_Check(chao, &valueCheck->StatCheck);

	case AppearanceRequirement:
		return Handle_Appearance_Check(chao, &valueCheck->AppearanceCheck);

	case AbilityRequirement:
		return Handle_Ability_Check(chao, &valueCheck->AbilityCheck);

	case ToyRequirement:
		return Handle_Toy_Check(chao, &valueCheck->ToyCheck);

	case RaceRequirement:
		return Handle_Race_Check(chao, &valueCheck->RaceCheck);

	case KarateRequirement:
		return Handle_Karate_Check(chao, &valueCheck->KarateCheck);

	case ClassesRequirement:
		return Handle_Classes_Check(chao, &valueCheck->ClassCheck);

	case LifetimeRequirement:
		return Handle_Lifetime_Check(chao, &valueCheck->LifetimeCheck);

	case BondRequirement:
		return Handle_Bond_Check(chao, &valueCheck->BondCheck);
	}

	return false;
}

bool Check_Mission_Requirement(MissionRequirement* requirement, CHAO_PARAM_GC* chao)
{
	ValueCheckPoint* currentPoint = requirement->CheckList;

	while (currentPoint)
	{
		for (size_t i = 0; i < currentPoint->Amount; i++)
		{
			if (!Handle_Check(chao, &currentPoint->Checks[i], requirement->Type))
			{
				break;
			}

			if (i == currentPoint->Amount - 1)
			{
				return true;
			}
		}

		currentPoint = currentPoint->OrCheck;
	}

	return false;
}

int Check_Mission_Requirements(MissionRequirements* requirements, CHAO_PARAM_GC* chao)
{
	int amountPassed = 0;

	for (size_t i = 0; i < requirements->Amount; i++)
	{
		if (Check_Mission_Requirement(&requirements->Requirements[i], chao))
		{
			amountPassed++;
		}
	}

	return amountPassed;
}

bool RequirementsPassed()
{
	CHAO_PARAM_GC* chao = AL_GBAManagerGetChaoData();
	
	if (!chao)
	{
		return false;
	}

	return Check_Mission_Requirements(Missions.MissionList[CurrentMission].Requirements, chao) == Missions.MissionList[CurrentMission].Requirements->Amount;
}

void Give_Mission_Reward(MissionReward* Reward)
{

}

void Display_Mission_Arrows(ObjectMaster* a1)
{
	SetChaoHUDThingBColor(1, 1, 1, 1);
	if (MissionCount >= 3 && CurrentMission > 0 && CurrentMission < MissionCount - 1)
	{
		//Draw normal arrows
		DrawChaoHudThingB(&MissionMenu[5], 70, 305, 100, 1, 1, 1, 1);
		DrawChaoHudThingB(&MissionMenu[6], 620, 305, 100, 1, 1, 1, 1);
	}
	else if (MissionCount >= 2 && CurrentMission == 0)
	{
		//Draw if no previous mission
		SetChaoHUDThingBColor(0.3f, 1, 1, 1);
		DrawChaoHudThingB(&MissionMenu[5], 70, 305, 100, 1, 1, 1, 1); //lower opacity
		SetChaoHUDThingBColor(1, 1, 1, 1);
		DrawChaoHudThingB(&MissionMenu[6], 620, 305, 100, 1, 1, 1, 1);
	}
	else if (MissionCount >= 2 && CurrentMission == MissionCount - 1)
	{
		//Draw if no next mission
		DrawChaoHudThingB(&MissionMenu[5], 70, 305, 100, 1, 1, 1, 1);
		SetChaoHUDThingBColor(0.3f, 1, 1, 1);
		DrawChaoHudThingB(&MissionMenu[6], 620, 305, 100, 1, 1, 1, 1); //lower opacity
	}
	else
	{
		//Draw if no previous or next mission
		SetChaoHUDThingBColor(0.3f, 1, 1, 1);
		DrawChaoHudThingB(&MissionMenu[5], 70, 305, 100, 1, 1, 1, 1); //lower opacity
		DrawChaoHudThingB(&MissionMenu[6], 620, 305, 100, 1, 1, 1, 1); //lower opacity
	}

}

bool Print_Mission_Requirements(MissionRequirements* requirements, CHAO_PARAM_GC* chao, size_t amountOfRequirements)
{
	
	std::string requirementString;

	for (int i = 0; i < amountOfRequirements; i++)
	{
		MissionRequirement* requirement = &requirements->Requirements[i];
		if (chao)
		{
			bool passed = Check_Mission_Requirement(requirement, chao);
			requirementString = std::string("- ") + requirement->RequirementDescription + ": " + (passed ? Checkmark : Denymark);
		}
		else
		{
			requirementString = std::string("- ") + requirement->RequirementDescription;
		}
		AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, requirementString.c_str(), TextLanguage == 0);
	}
}

void Print_Mission_Rewards(MissionRewards* rewards, size_t amountOfRewards, size_t amountOfRequirementsPassed = -1)
{
	std::string rewardString;

	for (int i = 0; i < amountOfRewards; i++)
	{
		MissionReward* reward = &rewards->Rewards[i];
		if (amountOfRequirementsPassed > -1)
		{
			bool passed = amountOfRequirementsPassed >= i;
			rewardString = std::string("- ") + reward->RewardDescription + ": " + (passed ? "Unlocked" : "Locked");
		}
		else 
		{
			rewardString = std::string("- ") + reward->RewardDescription;
		}
		AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, rewardString.c_str(), TextLanguage == 0);
	}
}

void Print_Mission_Text(ChaoMission* mission, size_t descriptionSize)
{
	std::string title = std::string("---------------------------\n") + mission->Name + "\n---------------------------";
	AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, title.c_str(), TextLanguage == 0);

	for (int i = 0; i < descriptionSize; i++)
	{
		AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, mission->Description->Lines[i], TextLanguage == 0);

		sub_543860();
	}
}

void Load_Mission_Text(int curMission, bool noText)
{
	AlMsgWarnRemove();
	AlMsgWarnCreate(93, 126.5, 450, 270.0);
	AlMsgWarnOpen();

	CHAO_PARAM_GC* chao = AL_GBAManagerGetChaoData();
	ChaoMission* mission = &Missions.MissionList[curMission];
	MissionDescription* Description = mission->Description;
	size_t descriptionSize = Description->AmountOfLines;
	size_t amountOfRequirements = mission->Requirements->Amount;
	size_t amountOfRewards = mission->Rewards->Amount;
	size_t amountOfBonusRequirements = mission->BonusRequirements ? mission->BonusRequirements->Amount : 0;
	size_t amountOfBonusRewards = mission->BonusRewards ? mission->BonusRewards->Amount : 0;
	int amountOfBonusRequirementsPassed = -1;
	if (!noText)
	{
		Print_Mission_Text(mission, descriptionSize);
	}

	AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, "\nRequirements:", TextLanguage == 0);
	Print_Mission_Requirements(mission->Requirements, chao, amountOfRequirements);

	sub_543860();

	AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, "\nRewards:", TextLanguage == 0);
	Print_Mission_Rewards(mission->Rewards, amountOfRewards);

	sub_543860();

	AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, "\nBonus Requirements:", TextLanguage == 0);
	Print_Mission_Requirements(mission->BonusRequirements, chao, amountOfBonusRequirements);

	sub_543860();

	if (chao)
	{
		amountOfBonusRequirementsPassed = Check_Mission_Requirements(mission->BonusRequirements, chao);
	}

	AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, "\nBonus Rewards:", TextLanguage == 0);
	Print_Mission_Rewards(mission->BonusRewards, amountOfBonusRewards, amountOfBonusRequirementsPassed);

	sub_52FB80();
}

void Mission_Arrow_Executor(ObjectMaster* a1)
{
	ODE_MENU_MASTER_WORK* al_master_data = AL_OdekakeMenuMaster_Data_ptr;

	if (a1->Data1.Entity->Action == 0)
	{
		if (al_master_data->EndFlag)
		{
			a1->MainSub = DeleteObject_;
			return;
		}
	}
}

void Load_Mission_Arrows()
{
	ObjectMaster* a1 = LoadObject(2, "Mission_Arrow_Executor", Mission_Arrow_Executor, LoadObj_Data1);
	a1->DisplaySub_Delayed1 = Display_Mission_Arrows;
}

void Handle_Mission_Menu(ODE_MENU_MASTER_WORK* OdeMenuMasterWork)
{
	ODE_MENU_MASTER_WORK* v1; // edi
	Uint32 press; // eax
	const char* v3; // edi
	CHAO_PARAM_GC* DataOfChao; // eax
	int timer; // eax
	ChaoMission* mission = &Missions.MissionList[CurrentMission];
	int amountPassed;
	int amountOfBonusPassed;

	v1 = AL_OdekakeMenuMaster_Data_ptr;
	if (AL_OdekakeMenuMaster_Data_ptr)
	{
		switch (AL_OdekakeMenuMaster_Data_ptr->mode)
		{
		case 0x0:
			LargeTitleBarExecutor_Load(AL_OdekakeMenuMaster_Data_ptr->CurrStage, 650.0, 66.0);
			Load_Mission_Arrows();
			CurrentMission = 0;
			mission = &Missions.MissionList[CurrentMission];
			AlMsgWarnCreate(93, 126.5, 450, 270.0);
			AlMsgWarnOpen();
			AL_OdeMenuSetMode(0, 0x1);
			break;
		case 0x1:
			Load_Mission_Text(0, false);
			AL_OdeMenuSetMode(0, 0x2);
			break;

		case 0x2:
			press = ControllerPointers[0]->press;
			if ((press & Buttons_A) != 0)
			{
				if (Al_MSGWarnKinderMessageArray[0].pkindercomessagething14->wordsLeftMaybe > 0)
				{
					goto LABEL_RETURN;
				}

				Load_Mission_Text(CurrentMission, false);
			}
			if ((press & Buttons_Y) != 0)
			{
				Load_Mission_Text(CurrentMission, true);
			}
			if ((press & Buttons_X) != 0)
			{
				if (RequirementsPassed())
				{
					AL_OdeMenuSetMode(0, 0x5);
				}
			}
			if ((press & Buttons_B) != 0)
			{
				AL_OdeMenuSetMode(0, 0x3);
				AL_OdekakeMenuMaster_Data_ptr->EndFlag = 1;
			}
			if ((press & Buttons_Left))
			{
				if (CurrentMission > 0)
				{
					CurrentMission--;
					Load_Mission_Text(CurrentMission, false);
				}
			}
			if ((press & Buttons_Right))
			{
				if (CurrentMission < MissionCount - 1)
				{
					CurrentMission++;
					Load_Mission_Text(CurrentMission, false);
				}
			}
			break;

		case 0x3:
			OdeMenuMasterWork->timer++;
			if (OdeMenuMasterWork->timer > 30)
			{
				OdeMenuMasterWork->timer = 0;
				AL_OdeMenuSetMode(0, 0x4);
			}
		case 0x4:
			if (OdeMenuMasterWork->state == 0)
			{
				AL_OdeMenuSetMode(0, 0xF);
			}
			break;
		case 0x5:

			break;
		case 0xF:
			v3 = "chao_r_e.ADX";
			PlayMusic(v3);
			ResetMusic();
			PlaySoundProbably(32787, 0, 0, 0);
			AlMsgWarnRemove();
			AL_OdekakeMenuMaster_Data_ptr->NextStage = 0;
			AL_OdeMenuChangeStage();
			goto LABEL_RETURN;
		default:
		LABEL_RETURN:
			dword_1A557C8 = 0;
			return;
		}
	}
}

ValueCheckPoint FittingInSwimChaoCheck3 = {
	.Amount = 1,
	.Checks = {
		{
			.ChaoTypeCheck =
			{
				.Type = ChaoTypeCheck,
				.Check = {
					.Type = {
						.value = ChaoType_Dark_Swim
					}
				}
			}
		}
	}
};

ValueCheckPoint FittingInSwimChaoCheck2 = {
	.Amount = 1,
	.OrCheck = &FittingInSwimChaoCheck3,
	.Checks = {
		{
			.ChaoTypeCheck =
			{
				.Type = ChaoTypeCheck,
				.Check = {
					.Type = {
						.value = ChaoType_Hero_Swim
					}
				}
			}
		}
	}
};

ValueCheckPoint FittingInSwimChaoCheck1 = {
	.Amount = 1,
	.OrCheck = &FittingInSwimChaoCheck2,
	.Checks = {
		{
			.ChaoTypeCheck =
			{
				.Type = ChaoTypeCheck,
				.Check = {
					.Type = {
						.value = ChaoType_Neutral_Swim
					}
				}
			}
		}
	}
};

ValueCheckPoint FittingInSwimLevelCheck = {
	.Amount = 1,
	.Checks = {
		{
			.StatCheck = {
				.Skill = ChaoSkill_Swim,
				.Type = LevelCheck,
				.Check = {
					.Level {
						.range = RangeType_MinRange,
						.min = 25
					}
				}
			}
		}
	}
};

ValueCheckPoint FittingInRunLevelCheck = {
	.Amount = 1,
	.Checks = {
		{
			.StatCheck = {
				.Skill = ChaoSkill_Run,
				.Type = LevelCheck,
				.Check = {
					.Level {
						.range = RangeType_MinRange,
						.min = 20
					}
				}
			}
		}
	}
};

ValueCheckPoint FittingInStaminaLevelCheck = {
	.Amount = 1,
	.Checks = {
		{
			.StatCheck = {
				.Skill = ChaoSkill_Stamina,
				.Type = LevelCheck,
				.Check = {
					.Level {
						.range = RangeType_MinRange,
						.min = 15
					}
				}
			}
		}
	}
};

ValueCheckPoint FittingInAnimalCheck4 = {
	.Amount = 2,
	.Checks =
	{
		{
			.AppearanceCheck = {
				.Type = AnimalArmsCheck,
				.Check = {
					.AnimalArms = {
						.value = Al_Animal_Seal
					}
				}
			}
		},
		{
			.AppearanceCheck = {
				.Type = AnimalTailCheck,
				.Check = {
					.AnimalTail = {
						.value = Al_Animal_Seal
					}
				}
			}
		}
	}
};

ValueCheckPoint FittingInAnimalCheck3 = {
	.Amount = 2,
	.OrCheck = &FittingInAnimalCheck4,
	.Checks =
	{
		{
			.AppearanceCheck = {
				.Type = AnimalArmsCheck,
				.Check = {
					.AnimalArms = {
						.value = Al_Animal_CWE_Seal
					}
				}
			}
		},
		{
			.AppearanceCheck = {
				.Type = AnimalTailCheck,
				.Check = {
					.AnimalTail = {
						.value = Al_Animal_CWE_Seal
					}
				}
			}
		}
	}
};

ValueCheckPoint FittingInAnimalCheck2 = {
	.Amount = 2,
	.OrCheck = &FittingInAnimalCheck3,
	.Checks =
	{
		{
			.AppearanceCheck = {
				.Type = AnimalArmsCheck,
				.Check = {
					.AnimalArms = {
						.value = Al_Animal_CWE_Seal
					}
				}
			}
		},
		{
			.AppearanceCheck = {
				.Type = AnimalTailCheck,
				.Check = {
					.AnimalTail = {
						.value = Al_Animal_Seal
					}
				}
			}
		}
	}
};

ValueCheckPoint FittingInAnimalCheck1 = {
	.Amount = 2,
	.OrCheck = &FittingInAnimalCheck2,
	.Checks =
	{
		{
			.AppearanceCheck = {
				.Type = AnimalArmsCheck,
				.Check = {
					.AnimalArms = {
						.value = Al_Animal_Seal
					}
				}
			}
		},
		{
			.AppearanceCheck = {
				.Type = AnimalTailCheck,
				.Check = {
					.AnimalTail = {
						.value = Al_Animal_CWE_Seal
					}
				}
			}
		}
	}
};

MissionRequirements FittingInRequirements = {
	5,
	{
		{
			TypeRequirement,
			&FittingInSwimChaoCheck1,
			"Adult Swim Chao"
		},
		{
			StatRequirement,
			&FittingInSwimLevelCheck,
			"Lvl 25 Swim"
		},
		{
			StatRequirement,
			&FittingInRunLevelCheck,
			"Lvl 20 Run"
		},
		{
			StatRequirement,
			&FittingInStaminaLevelCheck,
			"Lvl 15 Stamina"
		},
		{
			AppearanceRequirement,
			&FittingInAnimalCheck1,
			"Seal arms and tail"
		}
	}
};

MissionRewards FittingInRewards = {
	1,
	{
		RingReward, 25000, 1, "25,000 rings"
	}
};

ValueCheckPoint FittingInBonusCheck1 = {
	.Amount = 2,
	.Checks = {
		{
			.AppearanceCheck = {
				.Type = ToneCheck,
				.Check = {
					.Tone = {
						.value = ChaoTone_MonoTone
					}
				}
			}
		},
		{
			.AppearanceCheck = {
				.Type = ColorCheck,
				.Check = {
					.Color = {
						.value = ChaoColor_SkyBlue
					}
				}
			}
		}
	}
};

MissionRequirements FittingInBonusRequirements = {
	1,
	{
		{
			AppearanceRequirement,
			&FittingInBonusCheck1,
			"Monotone Sky Blue Chao"
		},
	}
};

MissionRewards FittingInBonusRewards = {
	1,
	{
		SeedReward, ChaoSeed_StrongSeed, 2, "2 Strong Seed"
	}
};

MissionDescription FittingInDescription = {
	8,
	{
		"\nDear Chao Raiser,",
		"\nI'm harold, a manager over at the Station Square Hotel. We moved rather recently and Henry, my 13-year-old son, has been struggling with the transition. With the new space, we have decided to adopt him a chao.",
		"\nHe is an active kid so we would like a chao that can keep up with and help him succeed.",
		"\nSwimming has been his passion for a long time, and now that he is older, he can join the swim team!",
		"We would like a physically active chao who could accompany him during training.",
		"\nHis favorite color is cyan, so I would like to surprise him with a light blue chao.",
		"\nThank you for taking our commission and we look forward to meeting our new companion.",
		"\n- Harold and Henry"
	}
};

ChaoMission FittingIn = {
	"Fitting In",
	&FittingInDescription,
	&FittingInRequirements,
	&FittingInRewards,
	&FittingInBonusRequirements,
	&FittingInBonusRewards
};

//MissionRequirements AWellEducatedChaoRequirements = {
//	3,
//	{
//		{AppearanceRequirement, [](RequirementValue* chaoValue) -> bool {return chaoValue->Appearance.Texture != SA2BTexture_None; }, "A textured chao"},
//		{ClassesRequirement, [](RequirementValue* chaoValue) -> bool {return chaoValue->Classroom.TotalClassesAttended >= 5; }, "Learned 5 lessons"},
//		{RaceRequirement, [](RequirementValue* chaoValue) -> bool {return (chaoValue->Race.MedalsEarned & MedalFlags_Challenge) == MedalFlags_Challenge; }, "Challenge medal won"}
//	}
//};
//
//EXTERN_C_START
//
//MissionRewards AWellEducatedChaoRewards = {
//	2,
//	{
//		{RingReward, 50000, 1, "50,000 rings"},
//		{
//			EggReward,
//			{
//				.Chao = {
//					ChaoColor_Normal,
//					SA2BTexture_YellowJewel,
//					ChaoTone_MonoTone,
//					ChaoShiny_None,
//					false
//				}
//			},
//			1,
//			"Gold Chao Egg"
//		}
//	}
//};
//
//EXTERN_C_END
//
//MissionDescription AWellEducatedChaoDescription = {
//		12,
//		{
//			"\n \n \n \nTo whom it may concern,",
//			"\n \nI am Ophelia Veridian, the co - owner of Casino Park.",
//			"\nThe \"grape vine\" has told me you are a quite the reliable breeder.\nMy family and I believe it is time to add a new addition into our lives.",
//			"\nFive wonderful chao already call our villa home and they all look forward to meeting their new sibling.\nRest assured that we take the utmost care of our chaos' health, safety, and entrichment.",
//			"There is a large pool, rockwall, and a spacious garden for them to explore and enjoy to their hearts content.",
//			"A private tutor comes twice a week, ensuring they are learning and growing as they should.",
//			"\nWe are looking for a well - educated, radient, and accomplished chao to join our small garden.",
//			"The chao must be able to take full advantage of the spaces provided, must have received several lessons to not fall behind, and be a champion racer.",
//			"I also have an inclination towards gemstone chao.",
//			"You will be compensated accordingly for the quality we are requesting and we look forward to meeting our new friend soon.",
//			"\nGood luck and take care,",
//			"\n \nOphelia Veridian.",
//		}
//};
//
//ChaoMission AWellEducatedChao = {
//	"A well educated chao",
//	&AWellEducatedChaoDescription,
//	&AWellEducatedChaoRequirements,
//	&AWellEducatedChaoRewards,
//	NULL,
//	NULL
//};
//
//MissionRequirements AFriendForLifeRequirements = {
//	3,
//	{
//		{StatRequirement, [](RequirementValue* chaoValue) -> bool {return chaoValue->Stat.Swim.Level >= 10 && chaoValue->Stat.Fly.Level >= 10 && chaoValue->Stat.Run.Level >= 10 && chaoValue->Stat.Power.Level >= 10 && chaoValue->Stat.Stamina.Level >= 10; }, "Lvl 10 in all base stats"},
//		{ClassesRequirement, [](RequirementValue* chaoValue) -> bool {return (chaoValue->Classroom.ClassesAttended & ChaoClassroomLessonFlags_SongLevel1) == ChaoClassroomLessonFlags_SongLevel1; }, "Attended a singing class"},
//		{StatRequirement, [](RequirementValue* chaoValue) -> bool {return chaoValue->Stat.Happiness == 100; }, "Max happiness"}
//	}
//};
//
//MissionRewards AFriendForLifeRewards = {
//	3,
//	{
//		{RingReward, 10000, 1, "10,000 rings"},
//		{FruitReward, SA2BFruit_HeartFruit, 4, "4 Heart Fruit"},
//		{AnimalReward, Al_Animal_SkeletonDog, 2, "2 Skeleton Dogs"}
//	}
//};
//
//MissionDescription AFriendForLifeDescription = {
//	9,
//	{
//		"\n \n \n \nHello,",
//		"\n \nMy name is Bailey Hills. I'm a classic, late twenty-something-year-old whose finally found a place to live and good enough job but I'll be honest, I'm pretty lonely.",
//		"\nAfter looking into it, a chao sounds like the perfect friend for me and my little apartment.",
//		"\nTheir little abilities really impress me! I'm someone who likes going hiking during the weekend so I'd appreciate it if they've gotten some decent skill in all their base abilities to keep up.",
//		"\nThey sing too, right? Please teach them to sing because I would love a duet partner. I don't really care how old they are or what they look like, but I do want to make sure they've been taken care of and they're friendly.",
//		"Please look after them.",
//		"\nThank you for your time, Chao Raiser. I'll make sure to send you payment as soon as they arrive. I'm so excited to meet my new buddy!",
//		"\nThanks a ton,",
//		"\n \nBailey Hills.",
//	}
//};
//
//ChaoMission AFriendForLife = {
//	"A friend for life",
//	&AFriendForLifeDescription,
//	&AFriendForLifeRequirements,
//	&AFriendForLifeRewards,
//	NULL,
//	NULL
//};
//
//MissionRequirements ExtraterresterialContactRequirements = {
//	4,
//	{
//		{LifetimeRequirement, [](RequirementValue* chaoValue) -> bool {return chaoValue->Lifetime.Reincarnations >= 1; }, "Reincarnated 1 time"},
//		{StatRequirement, [](RequirementValue* chaoValue) -> bool {return chaoValue->Stat.Happiness == 100; }, "Max happiness"},
//		{BondRequirement, [](RequirementValue* chaoValue) -> bool {return chaoValue->Bond.Sonic == 100 || chaoValue->Bond.Tails == 100 || chaoValue->Bond.Knuckles == 100 || chaoValue->Bond.Shadow == 100 || chaoValue->Bond.Rouge == 100; }, "Max bond with 1 character other than Eggman"},
//		{BondRequirement, [](RequirementValue* chaoValue) -> bool {return chaoValue->Bond.Eggman > -51 && chaoValue->Bond.Eggman < 51; }, "Neutral bond towards Eggman"}
//	}
//};
//
//MissionRewards ExtraterresterialContactRewards = {
//	1,
//	{
//		EggReward,
//		{
//			.Chao = {
//				ChaoColor_Normal,
//				SA2BTexture_Moon,
//				ChaoTone_MonoTone,
//				ChaoShiny_None,
//				false
//			}
//		},
//		1,
//		"Moon Chao"
//	}
//};
//
//MissionDescription ExtraterresterialContactDescription = {
//	6,
//	{
//		"\n \n \n \nGreetings,",
//		"\n \nI wOuLD LiiKe t0 REmaiN An0oNymOUS.ThERe HaVE beeN RumORs Of y0ur BreEdiNg aNd NURturING OppeRaTIOn.I AM rEQUestIng Aa chao fRom yoU.",
//		"\nmY OnLY ReQUirEmeNT Is thAt ThE child hAS bEeN nurTuRed aND gIVeN a W0ndErfUL LiFe.DO n0T woRRY For tHe wELLbeING Of The child, i lIve Am0ngST mAny HaPPY children On A SpaCiouS IslAND.",
//		"\nI Lo0k fORwARD tO heARiNG frOM You, hUmAn.",
//		"\n \n- AnOnYmOuS",
//		"\n \nP.S.KeEp child AwAy fRom RoBOtnIK.",
//	}
//};
//
//ChaoMission ExtraterresterialContact = {
//	"Contact from outer-space",
//	&ExtraterresterialContactDescription,
//	&ExtraterresterialContactRequirements,
//	NULL,
//	NULL,
//	NULL
//};

ChaoMissions Missions = {
	1,
	{
		FittingIn/*,
		AWellEducatedChao,
		AFriendForLife,
		ExtraterresterialContact*/
	}
};
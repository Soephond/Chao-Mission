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

char Checkmark = '✓';
char Denymark = '✗';

int CurrentMission = 0;
int MissionCount = Missions.Amount;

decltype(RequirementValue::Stat) Setup_Stat_Check(ChaoData* chao)
{
	return {
		.Swim =
		{
			(byte)chao->data.StatLevels[ChaoSkill_Swim],
			(StatGrade)chao->data.StatGrades[ChaoSkill_Swim],
			chao->data.StatPoints[ChaoSkill_Swim]
		},
			.Fly =
		{
			(byte)chao->data.StatLevels[ChaoSkill_Fly],
			(StatGrade)chao->data.StatGrades[ChaoSkill_Fly],
			chao->data.StatPoints[ChaoSkill_Fly]
		},
			.Run =
		{
			(byte)chao->data.StatLevels[ChaoSkill_Run],
			(StatGrade)chao->data.StatGrades[ChaoSkill_Run],
			chao->data.StatPoints[ChaoSkill_Run]
		},
			.Power =
		{
			(byte)chao->data.StatLevels[ChaoSkill_Power],
			(StatGrade)chao->data.StatGrades[ChaoSkill_Power],
			chao->data.StatPoints[ChaoSkill_Power]
		},
			.Stamina =
		{
			(byte)chao->data.StatLevels[ChaoSkill_Stamina],
			(StatGrade)chao->data.StatGrades[ChaoSkill_Stamina],
			chao->data.StatPoints[ChaoSkill_Stamina]
		},
			.Luck =
		{
			(byte)chao->data.StatLevels[ChaoSkill_Luck],
			(StatGrade)chao->data.StatGrades[ChaoSkill_Luck],
			chao->data.StatPoints[ChaoSkill_Luck]
		},
			.Intelligence =
		{
			(byte)chao->data.StatLevels[ChaoSkill_Intelligence],
			(StatGrade)chao->data.StatGrades[ChaoSkill_Intelligence],
			chao->data.StatPoints[ChaoSkill_Intelligence]
		},
			.Happiness = (__int8)chao->data.Happiness
	};
}

decltype(RequirementValue::Appearance) Setup_Appereance_Check(ChaoData* chao)
{
	return {
		.Color = (ChaoColor)chao->data.Color,
		.Tone = (ChaoTone)chao->data.MonotoneHighlights,
		.Shiny = (ChaoShiny)chao->data.Shiny,
		.Texture = (SA2BTexture)chao->data.Texture,
		.Negative = false,
		.AnimalParts = {
			(Al_Animal)chao->data.SA2BArmType,
			(Al_Animal)chao->data.SA2BEarType,
			(Al_Animal)chao->data.SA2BForeheadType,
			(Al_Animal)chao->data.SA2BHornType,
			(Al_Animal)chao->data.SA2BLegType,
			(Al_Animal)chao->data.SA2BTailType,
			(Al_Animal)chao->data.SA2BWingType,
			(Al_Animal)chao->data.SA2BFaceType
		}
	};

}

decltype(RequirementValue::Abilities) Setup_Abilities_Check(ChaoData* chao)
{
	int totalAbilities = 0;

	for (int i = 0; i < sizeof(__int16); i++)
	{
		if ((chao->data.SA2AnimalBehavior & (1 << i)) == 1)
		{
			totalAbilities++;
		}
	}

	return {
		.AbilitiesLearned = (__int16)chao->data.SA2AnimalBehavior,
		.TotalAbilitiesLearned = totalAbilities
	};
}

decltype(RequirementValue::Toys) Setup_Toys_Check(ChaoData* chao)
{
	int totalToys = 0;

	for (int i = 0; i < sizeof(__int16); i++)
	{
		if ((chao->data.SA2BToys & (1 << i)) == 1)
		{
			totalToys++;
		}
	}

	return {
		.ToysUnlocked = chao->data.SA2BToys,
		.TotalToysUnlocked = totalToys
	};
}

decltype(RequirementValue::Classroom) Setup_Classroom_Check(ChaoData* chao) 
{
	int totalAttended = 0;

	for (int i = 0; i < sizeof(int); i++)
	{
		if (((int)chao->data.ClassRoomFlags & (1 << i)) == 1)
		{
			totalAttended++;
		}
	}

	return {
		.ClassesAttended = (int)chao->data.ClassRoomFlags,
		.TotalClassesAttended = totalAttended,
	};
}

decltype(RequirementValue::Race) Setup_Race_Check(ChaoData* chao)
{
	int totalMedals = 0;

	for (int i = 0; i < sizeof(int); i++)
	{
		if (((int)chao->data.kara & (1 << i)) == 1)
		{
			totalMedals++;
		}
	}

	return {
		.MedalsEarned = (MedalFlags)chao->data.RaceMedals,
		.TotalMedals = totalMedals,
		.RacesWon = chao->data.RacesWon
	};
}

decltype(RequirementValue::Karate) Setup_Karate_Check(ChaoData* chao)
{

}

decltype(RequirementValue::Lifetime) Setup_Lifetime_Check(ChaoData* chao)
{
	return {
		.Age = short (chao->data.ClockRollovers / 60),
		.Reincarnations = chao->data.Reincarnations
	};
}

decltype(RequirementValue::Influence) Setup_Influence_Check(ChaoData* chao)
{

}

decltype(RequirementValue::Bond) Setup_Bond_Check(ChaoData* chao)
{

}

bool Check_Mission_Requirement(MissionRequirement* requirement)
{
		RequirementValue value;
		ChaoData* currentChao = AL_GBAManagerGetChaoData();

		switch (requirement->Type)
		{
			case TypeRequirement:
				value.Type = currentChao->data.Type;
				break;

			case StatRequirement:
				value.Stat = Setup_Stat_Check(currentChao);
				break;

			case AppearanceRequirement:
				value.Appearance = Setup_Appereance_Check(currentChao);
				break;

			case AbilityRequirement:
				value.Abilities = Setup_Abilities_Check(currentChao);
				break;

			case ToyRequirement:
				value.Toys = Setup_Toys_Check(currentChao);
				break;

			case ClassesRequirement:
				value.Classroom = Setup_Classroom_Check(currentChao);
				break;

			case RaceRequirement:
				value.Race = Setup_Race_Check(currentChao);
				break;

			case KarateRequirement:
				value.Karate = Setup_Karate_Check(currentChao);
				break;

			case LifetimeRequirement:
				value.Lifetime = Setup_Lifetime_Check(currentChao);
				break;

			case InfluenceRequirement:
				value.Influence = Setup_Influence_Check(currentChao);
				break;

			case BondRequirement:
				value.Bond = Setup_Bond_Check(currentChao);
				break;

			default:
				return false;
		}

		return requirement->RequirementCheck(&value);
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

void Load_Mission_Text(int curMission)
{
	if (Al_MSGWarnKinderMessageArray[0].pkindercomessagething14->wordsLeftMaybe > 0)
	{
		AlMsgWarnRemove();
		AlMsgWarnCreate(93, 126.5, 450, 270.0);
		AlMsgWarnOpen();
	}

	ChaoMission* mission = &Missions.MissionList[curMission];
	MissionDescription* Description = mission->Description;
	size_t descriptionSize = Description->AmountOfLines;
	size_t amountOfRequirements = mission->Requirements->Amount;
	size_t amountOfRewards = mission->Rewards->Amount;
	size_t amountOfBonusRequirements = mission->BonusRequirements ? mission->BonusRequirements->Amount : 0;
	size_t amountOfBonusRewards = mission->BonusRewards ? mission->BonusRewards->Amount : 0;
	std::string title = std::string("---------------------------\n") + mission->Name + "\n---------------------------";
	AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, title.c_str(), TextLanguage == 0);

	for (int i = 0; i < descriptionSize; i++)
	{
		AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, mission->Description->Lines[i], TextLanguage == 0);

		if (i + 1 == descriptionSize)
		{
			sub_52FB80();
		}
		else
		{
			sub_543860();
		}
	}

	for (int i = 0; i < amountOfRequirements; i++)
	{
		MissionRequirement* requirement = &mission->Requirements->Requirements[i];
		bool passed = Check_Mission_Requirement(requirement);
		std::string requirementString = std::string("- ") + requirement->RequirementDescription + ": " + (passed ? &Checkmark : &Denymark);
		AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, requirementString.c_str(), TextLanguage == 0);
	}
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
	ChaoData* DataOfChao; // eax
	AL_GBAManagerExecutor_Data* v5; // eax
	int timer; // eax
	int v7; // eax
	int v8; // eax
	int reward = 100;
	std::string rewardString;
	float x = 1.0f;

	v1 = AL_OdekakeMenuMaster_Data_ptr;
	if (AL_OdekakeMenuMaster_Data_ptr)
	{
		switch (AL_OdekakeMenuMaster_Data_ptr->mode)
		{
		case 0x0:
			LargeTitleBarExecutor_Load(AL_OdekakeMenuMaster_Data_ptr->CurrStage, 650.0, 66.0);
			Load_Mission_Arrows();
			CurrentMission = 0;
			AlMsgWarnCreate(93, 126.5, 450, 270.0);
			AlMsgWarnOpen();
			AL_OdeMenuSetMode(0, 0x1);
			break;
		case 0x1:
			Load_Mission_Text(0);
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
				AL_OdeMenuSetMode(0, 0x3);
				AL_OdekakeMenuMaster_Data_ptr->EndFlag = 1;
			}
			if ((press & Buttons_Left))
			{
				if (CurrentMission > 0)
				{
					CurrentMission--;
					Load_Mission_Text(CurrentMission);
				}
			}
			if ((press & Buttons_Right))
			{
				if (CurrentMission < MissionCount - 1)
				{
					CurrentMission++;
					Load_Mission_Text(CurrentMission);
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

MissionRequirements FittingInRequirements = {
	4,
	{
		{TypeRequirement, [](RequirementValue* chaoValue) -> bool {return chaoValue->Type == ChaoType_Dark_Swim || chaoValue->Type == ChaoType_Hero_Swim || chaoValue->Type == ChaoType_Neutral_Swim; }, "Adult Swim Chao"},
		{StatRequirement, [](RequirementValue* chaoValue) -> bool {return chaoValue->Stat.Swim.Level >= 25; }, "Lvl 25 Swim"},
		{StatRequirement, [](RequirementValue* chaoValue) -> bool {return chaoValue->Stat.Run.Level >= 20;  }, "Lvl 20 Run"},
		{StatRequirement, [](RequirementValue* chaoValue) -> bool {return chaoValue->Stat.Stamina.Level >= 15; }, "Lvl 15 Stamina"},
		{
			AppearanceRequirement,
			[](RequirementValue* chaoValue) -> bool {
				Al_Animal arms = chaoValue->Appearance.AnimalParts.Arms;
				Al_Animal tail = chaoValue->Appearance.AnimalParts.Tail;
				return (arms == Al_Animal_Seal || arms == Al_Animal_CWE_Seal) && (tail == Al_Animal_Seal || tail == Al_Animal_CWE_Seal);
			},
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

MissionRequirements FittingInBonusRequirements = {
	1,
	{
		{AppearanceRequirement, [](RequirementValue* chaoValue) -> bool {return chaoValue->Appearance.Color == ChaoColor_SkyBlue && chaoValue->Appearance.Tone == ChaoTone_MonoTone; }, "Monotone Light Blue Chao"},
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
		"\n\n\n\nDear Chao Raiser,",
		"\n\nI'm harold, a manager over at the Station Square Hotel. We moved rather recently and Henry, my 13-year-old son, has been struggling with the transition. With the new space, we have decided to adopt him a chao.",
		"\nHe is an active kid so we would like a chao that can keep up with and help him succeed.",
		"\nSwimming has been his passion for a long time, and now that he is older, he can join the swim team!",
		"We would like a physically active chao who could accompany him during training.",
		"\nHis favorite color is cyan, so I would like to surprise him with a light blue chao.",
		"\nThank you for taking our commission and we look forward to meeting our new companion.",
		"\n\n- Harold and Henry"
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

MissionRequirements AWellEducatedChaoRequirements = {
	3,
	{
		{AppearanceRequirement, [](RequirementValue* chaoValue) -> bool {return chaoValue->Appearance.Texture != SA2BTexture_None; }, "A textured chao"},
		{ClassesRequirement, [](RequirementValue* chaoValue) -> bool {return chaoValue->Classroom.TotalClassesAttended >= 5; }, "Learned 5 lessons"},
		{RaceRequirement, [](RequirementValue* chaoValue) -> bool {return (chaoValue->Race.MedalsEarned & MedalFlags_Challenge) == MedalFlags_Challenge; }, "Challenge medal won"}
	}
};

EXTERN_C_START

MissionRewards AWellEducatedChaoRewards = {
	2,
	{
		{RingReward, 50000, 1, "50,000 rings"},
		{
			EggReward,
			{
				.Chao = {
					ChaoColor_Normal,
					SA2BTexture_YellowJewel,
					ChaoTone_MonoTone,
					ChaoShiny_None,
					false
				} 
			},
			1, 
			"Gold Chao Egg"
		}
	}
};

EXTERN_C_END

MissionDescription AWellEducatedChaoDescription = {
		12,
		{
			"\n \n \n \nTo whom it may concern,",
			"\n \nI am Ophelia Veridian, the co - owner of Casino Park.",
			"\nThe \"grape vine\" has told me you are a quite the reliable breeder.\nMy family and I believe it is time to add a new addition into our lives.",
			"\nFive wonderful chao already call our villa home and they all look forward to meeting their new sibling.\nRest assured that we take the utmost care of our chaos' health, safety, and entrichment.",
			"There is a large pool, rockwall, and a spacious garden for them to explore and enjoy to their hearts content.",
			"A private tutor comes twice a week, ensuring they are learning and growing as they should.",
			"\nWe are looking for a well - educated, radient, and accomplished chao to join our small garden.",
			"The chao must be able to take full advantage of the spaces provided, must have received several lessons to not fall behind, and be a champion racer.",
			"I also have an inclination towards gemstone chao.",
			"You will be compensated accordingly for the quality we are requesting and we look forward to meeting our new friend soon.",
			"\nGood luck and take care,",
			"\n \nOphelia Veridian.",
		}
};

ChaoMission AWellEducatedChao = {
	"A well educated chao",
	&AWellEducatedChaoDescription,
	&AWellEducatedChaoRequirements,
	&AWellEducatedChaoRewards,
	NULL,
	NULL
};

MissionRequirements AFriendForLifeRequirements = {
	3,
	{
		{StatRequirement, [](RequirementValue* chaoValue) -> bool {return chaoValue->Stat.Swim.Level >= 10 && chaoValue->Stat.Fly.Level >= 10 && chaoValue->Stat.Run.Level >= 10 && chaoValue->Stat.Power.Level >= 10 && chaoValue->Stat.Stamina.Level >= 10; }, "Lvl 10 in all base stats"},
		{ClassesRequirement, [](RequirementValue* chaoValue) -> bool {return (chaoValue->Classroom.ClassesAttended & ChaoClassroomLessonFlags_SongLevel1) == ChaoClassroomLessonFlags_SongLevel1; }, "Attended a singing class"},
		{StatRequirement, [](RequirementValue* chaoValue) -> bool {return chaoValue->Stat.Happiness == 100; }, "Max happiness"}
	}
};

MissionRewards AFriendForLifeRewards = {
	3,
	{
		{RingReward, 10000, 1, "10,000 rings"},
		{FruitReward, SA2BFruit_HeartFruit, 4, "4 Heart Fruit"},
		{AnimalReward, Al_Animal_SkeletonDog, 2, "2 Skeleton Dogs"}
	}
};

MissionDescription AFriendForLifeDescription = {
	9,
	{
		"\n \n \n \nHello,",
		"\n \nMy name is Bailey Hills. I'm a classic, late twenty-something-year-old whose finally found a place to live and good enough job but I'll be honest, I'm pretty lonely.",
		"\nAfter looking into it, a chao sounds like the perfect friend for me and my little apartment.",
		"\nTheir little abilities really impress me! I'm someone who likes going hiking during the weekend so I'd appreciate it if they've gotten some decent skill in all their base abilities to keep up.",
		"\nThey sing too, right? Please teach them to sing because I would love a duet partner. I don't really care how old they are or what they look like, but I do want to make sure they've been taken care of and they're friendly.",
		"Please look after them.",
		"\nThank you for your time, Chao Raiser. I'll make sure to send you payment as soon as they arrive. I'm so excited to meet my new buddy!",
		"\nThanks a ton,",
		"\n \nBailey Hills.",
	}
};

ChaoMission AFriendForLife = {
	"A friend for life",
	&AFriendForLifeDescription,
	&AFriendForLifeRequirements,
	&AFriendForLifeRewards,
	NULL,
	NULL
};

MissionRequirements ExtraterresterialContactRequirements = {
	4,
	{
		{LifetimeRequirement, [](RequirementValue* chaoValue) -> bool {return chaoValue->Lifetime.Reincarnations >= 1; }, "Reincarnated 1 time"},
		{StatRequirement, [](RequirementValue* chaoValue) -> bool {return chaoValue->Stat.Happiness == 100; }, "Max happiness"},
		{BondRequirement, [](RequirementValue* chaoValue) -> bool {return chaoValue->Bond.Sonic == 100 || chaoValue->Bond.Tails == 100 || chaoValue->Bond.Knuckles == 100 || chaoValue->Bond.Shadow == 100 || chaoValue->Bond.Rouge == 100;}, "Max bond with 1 character other than Eggman"},
		{BondRequirement, [](RequirementValue* chaoValue) -> bool {return chaoValue->Bond.Eggman > -51 && chaoValue->Bond.Eggman < 51; }, "Neutral bond towards Eggman"}
	}
};

MissionRewards ExtraterresterialContactRewards = {
	1,
	{
		EggReward, 
		{
			.Chao = {
				ChaoColor_Normal,
				SA2BTexture_Moon,
				ChaoTone_MonoTone,
				ChaoShiny_None,
				false
			}
		},
		1, 
		"Moon Chao"
	}
};

MissionDescription ExtraterresterialContactDescription = {
	6,
	{
		"\n \n \n \nGreetings,",
		"\n \nI wOuLD LiiKe t0 REmaiN An0oNymOUS.ThERe HaVE beeN RumORs Of y0ur BreEdiNg aNd NURturING OppeRaTIOn.I AM rEQUestIng Aa chao fRom yoU.",
		"\nmY OnLY ReQUirEmeNT Is thAt ThE child hAS bEeN nurTuRed aND gIVeN a W0ndErfUL LiFe.DO n0T woRRY For tHe wELLbeING Of The child, i lIve Am0ngST mAny HaPPY children On A SpaCiouS IslAND.",
		"\nI Lo0k fORwARD tO heARiNG frOM You, hUmAn.",
		"\n \n- AnOnYmOuS",
		"\n \nP.S.KeEp child AwAy fRom RoBOtnIK.",
	}
};

ChaoMission ExtraterresterialContact = {
	"Contact from outer-space",
	&ExtraterresterialContactDescription,
	&ExtraterresterialContactRequirements,
	NULL,
	NULL,
	NULL
};

ChaoMissions Missions = {
	4, 
	{
		FittingIn,
		AWellEducatedChao,
		AFriendForLife,
		ExtraterresterialContact
	}
};
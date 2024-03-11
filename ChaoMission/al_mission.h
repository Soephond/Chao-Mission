#pragma once

#include "sa2modloader.h"

#include <functional>

#include "cwe_api.h"
#include "al_odekake.h"
#include "al_ui.h"
#include "al_chao_home.h"
#include "chao_data.h"

enum REWARD_TYPE {
	RingReward = 0,
	FruitReward = 1,
	AnimalReward = 2,
	SeedReward = 3,
	EggReward = 4,
	HatReward = 5,
	AccessoryReward = 6,
};

enum REQUIREMENT_TYPE
{
	TypeRequirement = 0,
	AppearanceRequirement = 1,
	StatRequirement = 2,
	AbilityRequirement = 3,
	ToyRequirement = 4,
	ClassesRequirement = 5,
	RaceRequirement = 6,
	KarateRequirement = 7,
	LifetimeRequirement = 8,
	InfluenceRequirement = 9,
	BondRequirement = 10,
};

struct ChaoStatValues {
	byte Level;
	StatGrade Grade;
	short Points;
};

struct AnimalPart {
	Al_Animal Arms;
	Al_Animal Ears;
	Al_Animal Forehead;
	Al_Animal Horn;
	Al_Animal Legs;
	Al_Animal Tail;
	Al_Animal Wings;
	Al_Animal Face;
};

union RequirementValue
{
	ChaoType Type;

	struct Appearance {
		ChaoColor Color;
		ChaoTone Tone;
		ChaoShiny Shiny;
		SA2BTexture Texture;
		bool Negative;
		AnimalPart AnimalParts;
	} Appearance;

	struct Stat {
		ChaoStatValues Swim;
		ChaoStatValues Fly;
		ChaoStatValues Run;
		ChaoStatValues Power;
		ChaoStatValues Stamina;
		ChaoStatValues Luck;
		ChaoStatValues Intelligence;
		__int8 Happiness;
	} Stat;

	struct Abilities {
		__int16 AbilitiesLearned;
		int TotalAbilitiesLearned;
	} Abilities;

	struct Toys {
		__int16 ToysUnlocked;
		int TotalToysUnlocked;
	} Toys;

	struct Classroom {
		__int32 ClassesAttended;
		int TotalClassesAttended;
	} Classroom;

	struct Race {
		MedalFlags MedalsEarned;
		__int8 TotalMedals;
		short RacesWon;
	} Race;

	struct Karate {
		KarateRank Rank;
		short MatchesWon;
	} Karate;

	struct Lifetime {
		short Age;
		short Reincarnations;
	} Lifetime;

	struct Influence {
		float SwimFlyInfluence;
		float RunPowerInfluence;
		float Magnitude;
		float Alignment;
	} Influence;

	struct Bond {
		__int8 Sonic;
		__int8 Tails;
		__int8 Knuckles;
		__int8 Shadow;
		__int8 Rouge;
		__int8 Eggman;
	} Bond;
};

union RewardValue
{
	int Rings;
	SA2BFruit Fruit;
	Al_Animal Animal;
	ChaoSeed Seed;
	SA2BHat Hat;
	char* AccessoryName;

	struct RewardChao {
		ChaoColor Color;
		SA2BTexture Texture;
		ChaoTone Tone;
		ChaoShiny Shiny;
		bool Negative;
	} Chao;
};

struct MissionReward
{
	REWARD_TYPE Type;
	RewardValue Value;
	int Amount = 1;
	const char* RewardDescription;
};

struct MissionRewards
{
	int Amount;
	MissionReward Rewards[];
};

struct MissionRequirement
{
	REQUIREMENT_TYPE Type;
	bool (*RequirementCheck)(RequirementValue*);
	const char* RequirementDescription;
};

struct MissionRequirements
{
	int Amount;
	MissionRequirement Requirements[];
};

struct MissionDescription
{
	int AmountOfLines;
	const char* Lines[];
};

struct ChaoMission
{
	const char* Name;
	MissionDescription* Description;
	MissionRequirements* Requirements;
	MissionRewards* Rewards;
	MissionRequirements* BonusRequirements;
	MissionRewards* BonusRewards;
};

struct ChaoMissions
{
	int Amount;
	ChaoMission MissionList[];
};

EXTERN_C_START
void Display_Mission_Arrows(ObjectMaster* a1);
void Load_Mission_Text(int curMission);
void Mission_Arrow_Executor(ObjectMaster* a1);
void Load_Mission_Arrows();
void Handle_Mission_Menu(ODE_MENU_MASTER_WORK* OdeMenuMasterWork);
bool Check_Mission_Requirement(MissionRequirement* Requirement);
void Give_Mission_Reward(MissionReward* Reward);
EXTERN_C_END

extern ChaoHudThingB MissionMenu[];
extern ChaoMissions Missions;
extern int CurrentMission;
extern int MissionCount;

static CWE_API_ODEKAKE_ENTRY MissionEntry = { Handle_Mission_Menu, nullptr, ODE_FLAGS_NONE, &MissionMenu[2],&MissionMenu[0],&MissionMenu[1],&MissionMenu[3],&MissionMenu[4], 1.0, 1.0, 0.5, 0.5 };
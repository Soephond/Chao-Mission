#pragma once

#include <optional>

#include "sa2modloader.h"

#include "chao_data.h"

enum REWARD_TYPE {
	RingReward = 0,
	FruitReward = 1,
	AnimalReward = 2,
	SeedReward = 3,
	EggReward = 4,
	HatReward = 5,
	ChaoReward = 6,
	//AccessoryReward = 7,
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
	BondRequirement = 10,
};

struct ChaoStatValues {
	byte Level;
	StatGrade Grade;
	short Points;
};

union RewardValue
{
	int Rings;
	SA2BFruit Fruit;
	Al_Animal Animal;
	ChaoSeed Seed;
	SA2BHat Hat;
	eCHAO_EGGS Egg;
	char* AccessoryName;

	struct RewardChao {
		char Name[7];
		eCHAO_TYPE Type;
		ChaoColor Color;
		SA2BTexture Texture;
		ChaoTone Tone;
		ChaoShiny Shiny;
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
	MissionReward* Rewards;
};

enum ValueCheckType {
	ChaoTypeCheck = 0,
	LevelCheck = 1,
	GradeCheck = 2,
	PointsCheck = 3,
	ColorCheck = 4,
	ToneCheck = 5,
	ShinyCheck = 6,
	TextureCheck = 7,
	AnimalArmsCheck = 8,
	AnimalEarsCheck = 9,
	AnimalForeheadCheck = 10,
	AnimalHornCheck = 11,
	AnimalLegsCheck = 12,
	AnimalTailCheck = 13,
	AnimalWingsCheck = 14,
	AnimalFaceCheck = 15,
	FlagCheck = 16,
	TotalCheck = 17,
	AmountWonCheck = 18,
	AgeCheck = 19,
	ReincarnationsCheck = 20,
	SwimFlyInfluenceCheck = 21,
	RunPowerInfluenceCheck = 22,
	AlignmentCheck = 23,
	MagnitudeCheck = 24,
	HappinessCheck = 25,
	CharacterLikeCheck = 26,
	CharacterFearCheck = 27,
	CharacterDistanceCheck = 28,
	CharacterMeetCheck = 29,
	KarateRankCheck = 30,
};

enum RangeType {
	RangeType_MinRange = 0,
	RangeType_MaxRange = 1,
	RangeType_Range = 2,
	RangeType_NoRange = 3,
};

template<typename T>
struct ValueCheckStruct {
	RangeType range = RangeType_NoRange;
	bool inverted = false;
	T value;
	T min;
	T max;
};



union ValueCheck {
	struct ChaoTypeCheck
	{
		ValueCheckType Type;

		union Type {
			ValueCheckStruct<ChaoType> ChaoType;
			ValueCheckStruct<float> SwimFlyInfluence;
			ValueCheckStruct<float> RunPowerInfluence;
			ValueCheckStruct<float> Alignment;
			ValueCheckStruct<float> Magnitude;
		} Check;
	}ChaoTypeCheck;

	struct StatCheck
	{
		ChaoSkill Skill;
		ValueCheckType Type;

		union Stat {
			ValueCheckStruct<ChaoGrade> Grade;
			ValueCheckStruct<short> Points;
			ValueCheckStruct<byte> Level;
			ValueCheckStruct<short> Happiness;
		} Check;
	}StatCheck;

	struct AppearanceCheck
	{
		ValueCheckType Type;

		union Appearance {
			ValueCheckStruct<ChaoColor> Color;
			ValueCheckStruct<ChaoTone> Tone;
			ValueCheckStruct<ChaoShiny> Shiny;
			ValueCheckStruct<SA2BTexture> Texture;
			ValueCheckStruct<Al_Animal> AnimalArms;
			ValueCheckStruct<Al_Animal> AnimalEars;
			ValueCheckStruct<Al_Animal> AnimalForehead;
			ValueCheckStruct<Al_Animal> AnimalHorn;
			ValueCheckStruct<Al_Animal> AnimalLegs;
			ValueCheckStruct<Al_Animal> AnimalTail;
			ValueCheckStruct<Al_Animal> AnimalWings;
			ValueCheckStruct<Al_Animal> AnimalFace;
		} Check;
	}AppearanceCheck;

	struct AbilityCheck
	{
		ValueCheckType Type;

		union Ability {
			ValueCheckStruct<__int16> AbilitiesLearned;
			ValueCheckStruct<int> TotalAbilitiesLearned;
		} Check;
	}AbilityCheck;

	struct ToyCheck
	{
		ValueCheckType Type;

		union Toy {
			ValueCheckStruct<__int16> ToysUnlocked;
			ValueCheckStruct<int> TotalToysUnlocked;
		} Check;
	} ToyCheck;

	struct ClassCheck
	{
		ValueCheckType Type;

		union Class {
			ValueCheckStruct<__int32> ClassesAttended;
			ValueCheckStruct<int> TotalClassesAttended;
		} Check;
	} ClassCheck;

	struct RaceCheck
	{
		ValueCheckType Type;

		union Race {
			ValueCheckStruct<MedalFlags> MedalsEarned;
			ValueCheckStruct<__int8> TotalMedals;
		} Check;
	} RaceCheck;

	struct KarateCheck
	{
		ValueCheckType Type;

		union Karate {
			ValueCheckStruct<KarateRank> Rank;
			ValueCheckStruct<short> MatchesWon;
		} Check;
	}KarateCheck;

	struct LifetimeCheck
	{
		ValueCheckType Type;

		union Lifetime {
			ValueCheckStruct<short> Age;
			ValueCheckStruct<short> Reincarnations;
		} Check;
	}LifetimeCheck;

	struct BondCheck
	{
		CharacterBondOrder Character;
		ValueCheckType Type;

		union Bond {
			ValueCheckStruct<Uint8> Like;
			ValueCheckStruct<Sint8> Fear;
			ValueCheckStruct<short> Distance;
			ValueCheckStruct<short> Meet;
		} Check;
	}BondCheck;
};

struct ValueCheckPoint
{
	int Amount;
	ValueCheckPoint* OrCheck = nullptr;
	ValueCheck* Checks;
};

struct MissionRequirement
{
	REQUIREMENT_TYPE Type;
	ValueCheckPoint CheckList;
	const char* RequirementDescription;
};

struct MissionRequirements
{
	int Amount;
	MissionRequirement* Requirements;
};

struct MissionDescription
{
	int AmountOfLines;
	const char** Lines;
};

struct ChaoMission
{
	const char* Name;
	MissionDescription Description;
	MissionRequirements Requirements;
	MissionRewards Rewards;
	MissionRequirements* BonusRequirements;
	MissionRewards* BonusRewards;
};

struct ChaoMissions
{
	int Amount;
	ChaoMission* MissionList;
};

inline void free_or_checks(const ValueCheckPoint& point)
{
	if(point.OrCheck != nullptr)
	{
		free_or_checks(*point.OrCheck);
		free(point.OrCheck);
	}
}

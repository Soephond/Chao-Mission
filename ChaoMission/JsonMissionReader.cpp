#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <magic_enum/magic_enum.hpp>
#include <string>
#include <windows.h>
#include <typeinfo>

#include "JsonMissionReader.h"

#include "al_helper.h"
#include "al_mission_structs.h"



typedef bool (*ValueCheckHandleFunc)(jsonValue&, ValueCheck*);

template <typename T, bool = std::is_enum<T>::value>
struct GetEnumHelper;

bool directoryExists(const std::string& path) {
	return std::filesystem::exists(path);
}

bool isInteger(const std::string& str) {
	try {
		size_t pos;
		int result = std::stoi(str, &pos);
		return pos == str.size();
	}
	catch (const std::invalid_argument&) {
		return false;
	}
	catch (const std::out_of_range&) {
		return false;
	}
}

bool GetMissionName(jsonValue& missionJson, const char*& name)
{
	if (!missionJson.HasMember("Name")) {
		return false;
	}

	std::string nameValue = missionJson["Name"].GetString();
	name = _strdup(nameValue.c_str());
	std::cout << "Mission Name: " << name << std::endl;
	return true;
}

bool GetMissionDescription(jsonValue& missionJson, MissionDescription* description)
{
	if (!missionJson.HasMember("Description")) {
		return false;
	}

	auto descriptionArray = missionJson["Description"].GetArray();
	description->AmountOfLines = (int)descriptionArray.Size();
	description->Lines = (const char**)malloc(description->AmountOfLines * sizeof(const char*));
	int i = 0;
	for (auto& descriptionLine : descriptionArray) {
		std::string line = descriptionLine.GetString();
		description->Lines[i] = _strdup(line.c_str());
		++i;
	}

	std::cout << "Mission Description: " << std::endl;
	for (int i = 0; i < description->AmountOfLines; i++) {
		std::cout << description->Lines[i] << std::endl;
	}

	return true;
}

template <typename T>
struct GetEnumHelper<T, true> {
	static bool Get(const jsonValue& valueJson, T& value) {
		if (valueJson.IsString()) {
			std::optional<T> readValue = magic_enum::enum_cast<T>(valueJson.GetString());

			if (!readValue.has_value()) {
				return false;
			}

			value = readValue.value();
			return true;
		}
		else if (valueJson.IsNumber()) {
			value = static_cast<T>(valueJson.GetInt());
			return true;
		}
		else {
			std::cout << "Invalid requirement type: " << valueJson.GetString() << std::endl;
			return false;
		}
	}
};

template <typename T>
struct GetEnumHelper<T, false> {
	static bool Get(const jsonValue& valueJson, T& value) {
		return false;
	}
};

template<typename T>
bool GetEnum(const jsonValue& valueJson, T& value) {
	return GetEnumHelper<T>::Get(valueJson, value);
}

bool HandleReward(REWARD_TYPE type, jsonValue& rewardJson, MissionReward*& reward)
{
	jsonValue value;

	switch (type)
	{

	case RingReward:

		if (!rewardJson.HasMember("Amount"))
		{
			return false;
		}

		if (!rewardJson["Amount"].IsNumber())
		{
			return false;
		}

		reward->Amount = rewardJson["Amount"].GetInt();

		break;

	case FruitReward:

		if (!(rewardJson.HasMember("Amount") && rewardJson.HasMember("Value"))) {
			return false;
		}

		if (!rewardJson["Amount"].IsNumber()) {
			return false;
		}

		if (!GetEnum<SA2BFruit>(rewardJson["Value"], reward->Value.Fruit))
		{
			return false;
		}

		reward->Amount = rewardJson["Amount"].GetInt();

		break;

	case AnimalReward:

		if (!(rewardJson.HasMember("Amount") && rewardJson.HasMember("Value"))) {
			return false;
		}

		if (!rewardJson["Amount"].IsNumber()) {
			return false;
		}

		if (!GetEnum<Al_Animal>(rewardJson["Value"], reward->Value.Animal))
		{
			return false;
		}

		reward->Amount = rewardJson["Amount"].GetInt();

		break;

	case SeedReward:

		if (!(rewardJson.HasMember("Amount") && rewardJson.HasMember("Value"))) {
			return false;
		}

		if (!rewardJson["Amount"].IsNumber()) {
			return false;
		}

		if (!GetEnum<ChaoSeed>(rewardJson["Value"], reward->Value.Seed))
		{
			return false;
		}

		reward->Amount = rewardJson["Amount"].GetInt();

		break;

	case ChaoReward:

		if (!rewardJson.HasMember("Value")) {
			return false;
		}

		value = rewardJson["Value"];

		if (value.HasMember("ChaoType"))
		{
			if (!GetEnum<eCHAO_TYPE>(value["ChaoType"], reward->Value.Chao.Type))
			{
				return false;
			}
		}
		else
		{
			reward->Value.Chao.Type = eCHAO_TYPE::Child;
		}

		if (value.HasMember("Color"))
		{
			if (!GetEnum<ChaoColor>(value["Color"], reward->Value.Chao.Color))
			{
				return false;
			}
		}
		else
		{
			reward->Value.Chao.Color = ChaoColor_Normal;
		}

		if (value.HasMember("Texture"))
		{
			if (!GetEnum<SA2BTexture>(value["Texture"], reward->Value.Chao.Texture))
			{
				return false;
			}
		}
		else
		{
			reward->Value.Chao.Texture = SA2BTexture_None;
		}

		if (value.HasMember("Tone"))
		{
			if (!GetEnum<ChaoTone>(value["Tone"], reward->Value.Chao.Tone))
			{
				return false;
			}
		}
		else
		{
			reward->Value.Chao.Tone = ChaoTone_MonoTone;
		}

		if (value.HasMember("Shiny"))
		{
			if (!GetEnum<ChaoShiny>(value["Shiny"], reward->Value.Chao.Shiny))
			{
				return false;
			}
		}
		else
		{
			reward->Value.Chao.Shiny = ChaoShiny_None;
		}

		if(value.HasMember("Name"))
		{
			int nameLength = 12;
			
			if(!value["Name"].IsString() || (int)value["Name"].GetStringLength() > nameLength)
			{
				return false;
			}
			
			EncodeChaoNameFromString(value["Name"].GetString(), reward->Value.Chao.Name, nameLength);
		}

		break;

	case HatReward:

		if (!(rewardJson.HasMember("Amount") && rewardJson.HasMember("Value"))) {
			return false;
		}

		if (!rewardJson["Amount"].IsNumber()) {
			return false;
		}

		if (!GetEnum<SA2BHat>(rewardJson["Value"], reward->Value.Hat))
		{
			return false;
		}

		reward->Amount = rewardJson["Amount"].GetInt();

		break;

	case EggReward:
	
		if (!(rewardJson.HasMember("Amount") && rewardJson.HasMember("Value"))) {
			return false;
		}

		if (!rewardJson["Amount"].IsNumber()) {
			return false;
		}

		if (!GetEnum<eCHAO_EGGS>(rewardJson["Value"], reward->Value.Egg))
		{
			return false;
		}

		reward->Amount = rewardJson["Amount"].GetInt();

		break;

	default:
		return false;
	}

	return true;
}

template<typename T>
bool GetVal(jsonValue& valueJson, ValueCheckStruct<T>& value)
{
	bool hasValue = valueJson.HasMember("Value");
	bool hasMaxValue = valueJson.HasMember("MaxValue");
	bool hasMinValue = valueJson.HasMember("MinValue");
	bool hasInverted = valueJson.HasMember("Inverted");

	value.inverted = false;
	value.range = RangeType_NoRange;
	value.value = T();
	value.max = T();
	value.min = T();

	try
	{
		if (hasValue) {
			if (std::is_enum<T>::value)
			{
				if (!GetEnum<T>(valueJson["Value"], value.value))
				{
					std::cout << "Invalid value type: " << valueJson["Value"].GetString() << std::endl;
					return false;
				}
			}
			else
			{
				value.value = T(valueJson["Value"].GetFloat());
			}
		}

		if (hasMaxValue) {

			if (std::is_enum<T>::value)
			{
				if (!GetEnum<T>(valueJson["MaxValue"], value.max))
				{
					std::cout << "Invalid maxvalue type: " << valueJson["MaxValue"].GetString() << std::endl;
					return false;
				}
			}
			else
			{
				value.max = T(valueJson["MaxValue"].GetFloat());
			}
		}

		if (hasMinValue) {

			if (std::is_enum<T>::value)
			{
				if (!GetEnum<T>(valueJson["MinValue"], value.min))
				{
					std::cout << "Invalid minvalue type: " << valueJson["MinValue"].GetString() << std::endl;
					return false;
				}
			}
			else
			{
				value.min = T(valueJson["MinValue"].GetFloat());
			}
		}

		if (!hasValue && !hasMaxValue && !hasMinValue)
		{
			std::cout << "Value, MaxValue and MinValue are all empty" << std::endl;
			return false;
		}

		if (hasInverted) {
			if (valueJson["Inverted"].IsBool())
			{
				value.inverted = valueJson["Inverted"].GetBool();
			}
			else
			{
				std::cout << "Invalid inverted type: " << valueJson["Inverted"].GetBool() << std::endl;
				return false;
			}
		}

		if (hasValue)
		{
			value.range = RangeType_NoRange;
		}
		else if (hasMaxValue && hasMinValue)
		{
			value.range = RangeType_Range;
		}
		else if (hasMaxValue)
		{
			value.range = RangeType_MaxRange;
		}
		else if (hasMinValue)
		{
			value.range = RangeType_MinRange;
		}
		else
		{
			std::cout << "Invalid range type" << std::endl;
			return false;
		}
	}
	catch (const std::exception& e)
	{
		std::cout << "Error: " << e.what() << std::endl;
		return false;
	}
	return true;
}

bool HandleTypeRequirementValueCheck(jsonValue& checkJson, ValueCheck* check)
{
	if (!checkJson.HasMember("Type"))
	{
		return false;
	}

	if (!GetEnum<ValueCheckType>(checkJson["Type"], check->ChaoTypeCheck.Type))
	{
		return false;
	}

	switch (check->ChaoTypeCheck.Type)
	{
	case ChaoTypeCheck:
		return GetVal<ChaoType>(checkJson, check->ChaoTypeCheck.Check.ChaoType);

	case SwimFlyInfluenceCheck:
		return GetVal<float>(checkJson, check->ChaoTypeCheck.Check.SwimFlyInfluence);

	case RunPowerInfluenceCheck:
		return GetVal<float>(checkJson, check->ChaoTypeCheck.Check.RunPowerInfluence);

	case AlignmentCheck:
		return GetVal<float>(checkJson, check->ChaoTypeCheck.Check.Alignment);

	case MagnitudeCheck:
		return GetVal<float>(checkJson, check->ChaoTypeCheck.Check.Magnitude);
	}

	return false;
}

bool HandleStatRequirementValueCheck(jsonValue& checkJson, ValueCheck* check)
{
	std::cout << "Handling Stat Requirement" << std::endl;

	if (!checkJson.HasMember("Type"))
	{
		std::cout << "Check does not contain type" << std::endl;
		return false;
	}

	if (!GetEnum<ValueCheckType>(checkJson["Type"], check->StatCheck.Type))
	{
		std::cout << "Failed to get type enum" << std::endl;
		return false;
	}

	if (check->StatCheck.Type != HappinessCheck)
	{
		if (!checkJson.HasMember("Skill"))
		{
			std::cout << "Check does not contain skill" << std::endl;
			return false;
		}

		if (!GetEnum<ChaoSkill>(checkJson["Skill"], check->StatCheck.Skill))
		{
			std::cout << "Failed to get skill enum" << std::endl;
			return false;
		}
	}

	std::cout << "Getting value" << std::endl;

	switch (check->StatCheck.Type)
	{
	case GradeCheck:
		return GetVal<ChaoGrade>(checkJson, check->StatCheck.Check.Grade);

	case PointsCheck:
		return GetVal<short>(checkJson, check->StatCheck.Check.Points);

	case LevelCheck:
		return GetVal<byte>(checkJson, check->StatCheck.Check.Level);

	case HappinessCheck:
		return GetVal<short>(checkJson, check->StatCheck.Check.Happiness);
	}

	return false;
}

bool HandleAppearanceRequirementValueCheck(jsonValue& checkJson, ValueCheck* check)
{
	if (!checkJson.HasMember("Type"))
	{
		return false;
	}

	if (!GetEnum<ValueCheckType>(checkJson["Type"], check->AppearanceCheck.Type))
	{
		return false;
	}

	switch (check->AppearanceCheck.Type)
	{
	case ColorCheck:
		return GetVal<ChaoColor>(checkJson, check->AppearanceCheck.Check.Color);

	case ToneCheck:
		return GetVal<ChaoTone>(checkJson, check->AppearanceCheck.Check.Tone);

	case ShinyCheck:
		return GetVal<ChaoShiny>(checkJson, check->AppearanceCheck.Check.Shiny);

	case TextureCheck:
		return GetVal<SA2BTexture>(checkJson, check->AppearanceCheck.Check.Texture);

	case AnimalArmsCheck:
		return GetVal<Al_Animal>(checkJson, check->AppearanceCheck.Check.AnimalArms);

	case AnimalEarsCheck:
		return GetVal<Al_Animal>(checkJson, check->AppearanceCheck.Check.AnimalEars);

	case AnimalForeheadCheck:
		return GetVal<Al_Animal>(checkJson, check->AppearanceCheck.Check.AnimalForehead);

	case AnimalHornCheck:
		return GetVal<Al_Animal>(checkJson, check->AppearanceCheck.Check.AnimalHorn);

	case AnimalLegsCheck:
		return GetVal<Al_Animal>(checkJson, check->AppearanceCheck.Check.AnimalLegs);

	case AnimalTailCheck:
		return GetVal<Al_Animal>(checkJson, check->AppearanceCheck.Check.AnimalTail);

	case AnimalWingsCheck:
		return GetVal<Al_Animal>(checkJson, check->AppearanceCheck.Check.AnimalWings);

	case AnimalFaceCheck:
		return GetVal<Al_Animal>(checkJson, check->AppearanceCheck.Check.AnimalFace);
	}

	return false;
}

bool HandleAbilityRequirementValueCheck(jsonValue& checkJson, ValueCheck* check)
{
	if (!checkJson.HasMember("Type"))
	{
		return false;
	}

	if (!GetEnum<ValueCheckType>(checkJson["Type"], check->AbilityCheck.Type))
	{
		return false;
	}

	switch (check->AbilityCheck.Type)
	{
	case TotalCheck:
		return GetVal<int>(checkJson, check->AbilityCheck.Check.TotalAbilitiesLearned);

	case FlagCheck:
		return GetVal<__int16>(checkJson, check->AbilityCheck.Check.AbilitiesLearned);
	}

	return false;
}

bool HandleToyRequirementValueCheck(jsonValue& checkJson, ValueCheck* check)
{
	if (!checkJson.HasMember("Type"))
	{
		return false;
	}

	if (!GetEnum<ValueCheckType>(checkJson["Type"], check->ToyCheck.Type))
	{
		return false;
	}

	switch (check->ToyCheck.Type)
	{
	case TotalCheck:
		return GetVal<int>(checkJson, check->ToyCheck.Check.TotalToysUnlocked);

	case FlagCheck:
		return GetVal<__int16>(checkJson, check->ToyCheck.Check.ToysUnlocked);
	}

	return false;
}

bool HandleClassesRequirementValueCheck(jsonValue& checkJson, ValueCheck* check)
{
	if (!checkJson.HasMember("Type"))
	{
		return false;
	}

	if (!GetEnum<ValueCheckType>(checkJson["Type"], check->ClassCheck.Type))
	{
		return false;
	}

	switch (check->ClassCheck.Type)
	{
	case TotalCheck:
		return GetVal<int>(checkJson, check->ClassCheck.Check.TotalClassesAttended);

	case FlagCheck:
		return GetVal<__int32>(checkJson, check->ClassCheck.Check.ClassesAttended);
	}

	return false;
}

bool HandleRaceRequirementValueCheck(jsonValue& checkJson, ValueCheck* check)
{
	if (!checkJson.HasMember("Type"))
	{
		return false;
	}

	if (!GetEnum<ValueCheckType>(checkJson["Type"], check->RaceCheck.Type))
	{
		return false;
	}

	switch (check->RaceCheck.Type)
	{
	case TotalCheck:
		return GetVal<__int8>(checkJson, check->RaceCheck.Check.TotalMedals);

	case FlagCheck:
		return GetVal<MedalFlags>(checkJson, check->RaceCheck.Check.MedalsEarned);
	}

	return false;
}

bool HandleKarateRequirementValueCheck(jsonValue& checkJson, ValueCheck* check)
{
	if (!checkJson.HasMember("Type"))
	{
		return false;
	}

	if (!GetEnum<ValueCheckType>(checkJson["Type"], check->KarateCheck.Type))
	{
		return false;
	}

	switch (check->KarateCheck.Type)
	{
	case KarateRankCheck:
		return GetVal<KarateRank>(checkJson, check->KarateCheck.Check.Rank);

	case AmountWonCheck:
		return GetVal<short>(checkJson, check->KarateCheck.Check.MatchesWon);
	}

	return false;
}

bool HandleLifetimeRequirementValueCheck(jsonValue& checkJson, ValueCheck* check)
{
	if (!checkJson.HasMember("Type"))
	{
		return false;
	}

	if (!GetEnum<ValueCheckType>(checkJson["Type"], check->LifetimeCheck.Type))
	{
		return false;
	}

	switch (check->LifetimeCheck.Type)
	{
	case AgeCheck:
		return GetVal<short>(checkJson, check->LifetimeCheck.Check.Age);

	case ReincarnationsCheck:
		return GetVal<short>(checkJson, check->LifetimeCheck.Check.Reincarnations);
	}

	return false;
}

bool HandleBondRequirementValueCheck(jsonValue& checkJson, ValueCheck* check)
{
	if (!checkJson.HasMember("Type"))
	{
		return false;
	}

	if (!GetEnum<ValueCheckType>(checkJson["Type"], check->BondCheck.Type))
	{
		return false;
	}

	if (!checkJson.HasMember("Character"))
	{
		return false;
	}

	if (!GetEnum<CharacterBondOrder>(checkJson["Character"], check->BondCheck.Character))
	{
		return false;
	}

	switch (check->BondCheck.Type)
	{
	case CharacterDistanceCheck:
		return GetVal<short>(checkJson, check->BondCheck.Check.Distance);

	case CharacterMeetCheck:
		return GetVal<short>(checkJson, check->BondCheck.Check.Meet);

	case CharacterLikeCheck:
		return GetVal<Uint8>(checkJson, check->BondCheck.Check.Like);

	case CharacterFearCheck:
		return GetVal<Sint8>(checkJson, check->BondCheck.Check.Fear);
	}

	return false;
}

ValueCheckHandleFunc GetHandleFunc(REQUIREMENT_TYPE& type)
{
	switch (type)
	{
	case TypeRequirement:
		return &HandleTypeRequirementValueCheck;

	case StatRequirement:
		return &HandleStatRequirementValueCheck;

	case AppearanceRequirement:
		return &HandleAppearanceRequirementValueCheck;

	case AbilityRequirement:
		return &HandleAbilityRequirementValueCheck;

	case ToyRequirement:
		return &HandleToyRequirementValueCheck;

	case ClassesRequirement:
		return &HandleClassesRequirementValueCheck;

	case RaceRequirement:
		return &HandleRaceRequirementValueCheck;

	case KarateRequirement:
		return &HandleKarateRequirementValueCheck;

	case LifetimeRequirement:
		return &HandleLifetimeRequirementValueCheck;

	case BondRequirement:
		return &HandleBondRequirementValueCheck;
	}

	return nullptr;
}

bool GetReward(jsonValue& rewardJson, MissionReward*& reward)
{
	if (!(rewardJson.HasMember("Description") && rewardJson.HasMember("Type"))) {
		return false;
	}

	if (!GetEnum<REWARD_TYPE>(rewardJson["Type"], reward->Type)) {
		return false;
	}

	if (!HandleReward(reward->Type, rewardJson, reward))
	{
		return false;
	}

	std::string description = rewardJson["Description"].GetString();
	reward->RewardDescription = _strdup(description.c_str());

	std::cout << "Reward Type: " << magic_enum::enum_name(reward->Type) << ", Amount: " << (reward->Type == RingReward ? reward->Value.Rings : reward->Amount) << ", Description: " << reward->RewardDescription << std::endl;

	return true;
}

bool GetRewards(jsonValue& rewardsJson, MissionRewards& rewards)
{
	if (rewardsJson.IsNull()) {
		return false;
	}

	std::vector<MissionReward> rewardsVector = std::vector<MissionReward>();
	int i = 0;

	for (auto& reward : rewardsJson.GetArray()) {
		MissionReward* newReward = new MissionReward();
		if (GetReward(reward, newReward)) {
			rewardsVector.push_back(*newReward);
			++i;
		}
		else {
			return false;
		}
	}
	
	rewards.Amount = (int)rewardsVector.size();
	rewards.Rewards = (MissionReward*)malloc(sizeof(MissionReward) * rewardsVector.size());
	memcpy(rewards.Rewards, rewardsVector.data(), sizeof(MissionReward) * rewardsVector.size());

	return true;
}

bool GetCheck(ValueCheckHandleFunc& handlerFunc, jsonValue& checkJson, ValueCheck*& check)
{
	std::cout << "Getting check for handler" << std::endl;

	if (!(checkJson.HasMember("Type"))) {
		std::cout << "Check does not contain type or value" << std::endl;
		return false;
	}

	if (!handlerFunc(checkJson, check)) {
		std::cout << "Failed to handle func" << std::endl;
		return false;
	}

	return true;
}

bool GetRequirementChecksFromJson(REQUIREMENT_TYPE& baseType, jsonValue& checksJson, ValueCheckPoint* checkPoint)
{
	if (checksJson.IsNull()) {
		return false;
	}

	std::vector<ValueCheck> checksVector = std::vector<ValueCheck>();
	int i = 0;

	for (auto& check : checksJson.GetArray()) {
		ValueCheck* newCheck;
		ValueCheckHandleFunc handlerFunc = GetHandleFunc(baseType);
		if (handlerFunc == nullptr)
		{
			std::cout << "Null Pointer Handler Function." << std::endl;
			return false;
		}
		if (!GetCheck(handlerFunc, check, newCheck))
		{
			std::cout << "Failed to get check" << std::endl;
			return false;
		}

		checksVector.push_back(*newCheck);
	}

	if (checksVector.empty())
	{
		return true;
	}

	checkPoint->Amount = (int)checksVector.size();
	checkPoint->Checks = (ValueCheck*)malloc(sizeof(ValueCheck) * checksVector.size());
	memcpy(checkPoint->Checks, checksVector.data(), sizeof(ValueCheck) * checksVector.size());

	return true;
}

bool GetMissionRequirementFromJson(jsonValue& requirementJson, MissionRequirement* requirement)
{
	if (!(requirementJson.HasMember("Type") && requirementJson.HasMember("Description") && requirementJson.HasMember("Checks"))) {
		return false;
	}

	if (!GetEnum<REQUIREMENT_TYPE>(requirementJson["Type"], requirement->Type)) {
		return false;
	}

	auto checksArray = requirementJson["Checks"].GetArray();
	int step = 1;

	ValueCheckPoint* checkPoint = &requirement->CheckList;

	for (auto& checkList : checksArray)
	{
		if (!checkList.IsArray())
		{
			return false;
		}


		if (!GetRequirementChecksFromJson(requirement->Type, checkList, checkPoint))
		{
			return false;
		}
		
		if (step < (int)checksArray.Size())
		{
			checkPoint->OrCheck = (ValueCheckPoint*)malloc(sizeof(ValueCheckPoint));
			checkPoint = checkPoint->OrCheck;
		}
		else
		{
			checkPoint->OrCheck = nullptr;
		}

		step++;
	}

	std::string description = requirementJson["Description"].GetString();
	requirement->RequirementDescription = _strdup(description.c_str());

	std::cout << "Requirement Type: " << magic_enum::enum_name(requirement->Type) << ", " << requirement->RequirementDescription << std::endl;

	return true;
}

bool GetMissionRequirementsFromJson(jsonValue& requirementsJson, MissionRequirements& requirements)
{
	
	if (requirementsJson.IsNull()) {
		return false;
	}

	std::vector<MissionRequirement> requirementsVector = std::vector<MissionRequirement>();
	int i = 0;

	for (auto& requirement : requirementsJson.GetArray()) {
		MissionRequirement newRequirement;
		if (GetMissionRequirementFromJson(requirement, &newRequirement)) {
			requirementsVector.push_back(newRequirement);
			++i;
		}
		else {
			return false;
		}
	}
	
	requirements.Amount = (int)requirementsVector.size();
	requirements.Requirements = (MissionRequirement*)malloc(sizeof(MissionRequirement) * requirementsVector.size());
	memcpy(requirements.Requirements, requirementsVector.data(), sizeof(MissionRequirement) * requirementsVector.size());

	return true;
}

ChaoMission LoadChaoMissionFromJson(jsonDocument& missionJson) {

	ChaoMission mission = {};

	if (!GetMissionName(missionJson, mission.Name))
	{
		std::cout << "Mission does not contain a name" << std::endl;
		throw std::runtime_error("Mission does not contain a name");
	}
	if (!GetMissionDescription(missionJson, &mission.Description))
	{
		std::cout << "Mission does not contain a description" << std::endl;
		throw std::runtime_error("Mission does not contain a description");
	}
	if (!GetMissionRequirementsFromJson(missionJson["Requirements"], mission.Requirements))
	{
		std::cout << "Mission does not contain requirements" << std::endl;
		throw std::runtime_error("Mission does not contain requirements");
	}
	if (!GetRewards(missionJson["Rewards"], mission.Rewards))
	{
		std::cout << "Mission does not contain rewards" << std::endl;
		throw std::runtime_error("Mission does not contain rewards");
	}
	mission.BonusRequirements = (MissionRequirements*)malloc(sizeof(MissionRequirements));
	if (!GetMissionRequirementsFromJson(missionJson["Bonus Requirements"], *mission.BonusRequirements))
	{
		std::cout << "Mission does not contain bonus requirements" << std::endl;
		free(mission.BonusRequirements);
		mission.BonusRequirements = nullptr;
	}
	mission.BonusRewards = (MissionRewards*)malloc(sizeof(MissionRewards));
	if (!GetRewards(missionJson["Bonus Rewards"], *mission.BonusRewards))
	{
		std::cout << "Mission does not contain bonus rewards" << std::endl;
		free(mission.BonusRewards);
		mission.BonusRewards = nullptr;
	}

	return mission;
}
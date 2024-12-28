// ReSharper disable CppClangTidyBugproneNarrowingConversions
#include "pch.h"
#include "al_mission.h"
#include "al_inv.h"

#include <utility>
#include <cstring>

#include "al_chao_home.h"
#include "chao_data.h"
#include "MissionFileHandler.h"

ChaoMissions missions;
std::vector<std::string> MissionScreenText;

ChaoHudThingB MissionMenu[] = {
    {1, 128, 32, 22.0f / 1024.0f, 297.0f / 1024.0f, 320.0f / 1024.0f, 364.0f / 1024.0f, AL_ODE_CHAO_TEXLIST, 0}, //text
    {1, 128, 32, 22.0f / 1024.0f, 297.0f / 1024.0f, 320.0f / 1024.0f, 364.0f / 1024.0f, AL_ODE_CHAO_TEXLIST, 0},
    //grey text
    {1, 40, 40, 410.0f / 1024.0f, 36.0f / 1024.0f, 562.0f / 1024.0f, 188.0f / 1024.0f, AL_ODE_CHAO_TEXLIST, 0}, //icon
    {1, 50, 50, 610.0f / 1024.0f, 30.0f / 1024.0f, 751.0f / 1024.0f, 193.0f / 1024.0f, AL_ODE_CHAO_TEXLIST, 0},
    //icon black and white
    {1, 185, 46, 22.0f / 1024.0f, 297.0f / 1024.0f, 320.0f / 1024.0f, 364.0f / 1024.0f, AL_ODE_CHAO_TEXLIST, 0},
    //text titlebar
    {1, 52.5, 90, 73.0f / 1024.0f, 756.0f / 1024.0f, 173.0f / 1024.0f, 940.0f / 1024.0f, AL_ODE_CHAO_TEXLIST, 0},
    //left Arrow
    {1, 52.5, 90, 215.0f / 1024.0f, 756.0f / 1024.0f, 315.0f / 1024.0f, 940.0f / 1024.0f, AL_ODE_CHAO_TEXLIST, 0},
    //right Arrow
    {1, 52.5, 90, 336.0f / 1024.0f, 756.0f / 1024.0f, 436.0f / 1024.0f, 940.0f / 1024.0f, AL_ODE_CHAO_TEXLIST, 0},
    //left Arrow inactive
    {1, 52.5, 90, 467.0f / 1024.0f, 756.0f / 1024.0f, 577.0f / 1024.0f, 940.0f / 1024.0f, AL_ODE_CHAO_TEXLIST, 0},
    //right Arrow inactive
};

std::string Checkmark = "Cleared";
std::string Denymark = "Not Cleared";

int CurrentMission = 0;
int CurrentLine = 0;
int MissionCount = 0;

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

template <typename T>
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
        return Check_Value(info->Check.ChaoType, (ChaoType)chao->Type);

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
    int classes = chao->knowledge.ArtFlag | (chao->knowledge.DanceFlag << 8) | (chao->knowledge.SongFlag << 16) | (chao
        ->knowledge.MusicFlag << 24);
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

void LoadMissions()
{
    missions.Amount = GetAmountOfActiveMissions();
    missions.MissionList = (ChaoMission*)malloc(sizeof(ChaoMission) * missions.Amount);
    memcpy(missions.MissionList, GetActiveMissions().data(), sizeof(ChaoMission) * missions.Amount);
}

void UnloadMissions()
{
    for (int i = 0; i < missions.Amount; i++)
    {
        auto mission = &missions.MissionList[i];

        for (int j = 0; j < mission->Description.AmountOfLines; j++)
        {
            auto line = mission->Description.Lines[j];
            
            free(const_cast<char*>(line));
        }

        for (int j = 0; j < mission->Requirements.Amount; j++)
        {
            auto requirement = &mission->Requirements.Requirements[j];
            
            free(const_cast<char*>(requirement->RequirementDescription));
            free(requirement->CheckList.Checks);
            free_or_checks(requirement->CheckList);
        }

        for (int j = 0; j < mission->Rewards.Amount; j++)
        {
            auto reward = &mission->Rewards.Rewards[j];

            free(const_cast<char*>(reward->RewardDescription));
        }

        if(mission->BonusRequirements)
        {
            for (int j = 0; j < mission->BonusRequirements->Amount; j++)
            {
                auto requirement = &mission->BonusRequirements->Requirements[j];
                
                free(const_cast<char*>(requirement->RequirementDescription));
                free(requirement->CheckList.Checks);
                free_or_checks(requirement->CheckList);
            }
        }

        if (mission->BonusRewards)
        {
            for (int j = 0; j < mission->BonusRewards->Amount; j++)
            {
                auto reward = &mission->Rewards.Rewards[j];

                free(const_cast<char*>(reward->RewardDescription));
            }
        }
        
        free(const_cast<char*>(mission->Name));
        free(mission->BonusRequirements);
        free(mission->BonusRewards);
    }
    
    missions.Amount = 0;
    free(missions.MissionList);
}

bool Check_Mission_Requirement(MissionRequirement* requirement, CHAO_PARAM_GC* chao)
{
    ValueCheckPoint* currentPoint = &requirement->CheckList;

    while (currentPoint)
    {
        for (int i = 0; i < currentPoint->Amount; i++)
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

int Check_Mission_Requirements(MissionRequirements*& requirements, CHAO_PARAM_GC* chao)
{
    int amountPassed = 0;

    for (int i = 0; i < requirements->Amount; i++)
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

    auto* requirements = &missions.MissionList[CurrentMission].Requirements;

    return Check_Mission_Requirements(requirements, chao) == missions.MissionList[CurrentMission].Requirements.Amount;
}

void Give_Reward(MissionReward*& reward)
{
    switch (reward->Type)
    {
    case RingReward:
        //Give rings
        RingDisplay_AddRings(RingDisplayData_.field_C + reward->Amount);
        break;

    case FruitReward:
        //Give fruit
        AddItemsToInventory(ChaoItemCategory_Fruit, reward->Value.Fruit, reward->Amount);
        break;

    case AnimalReward:
        //Give animal
        AddMinimalsToInventory(playerpwp[0]->PlayerNum, reward->Value.Animal,
                               reward->Amount);
        break;

    case SeedReward:
        //Give seed
        AddItemsToInventory(ChaoItemCategory_Seed, reward->Value.Seed, reward->Amount);
        break;

    case EggReward:
        //Give egg
        AddItemsToInventory(ChaoItemCategory_Egg, (short)reward->Value.Egg, reward->Amount);
        break;

    case HatReward:
        //Give hat
        AddItemsToInventory(ChaoItemCategory_Hat, reward->Value.Hat, reward->Amount);
        break;

    case ChaoReward:
        //Give free chao slot
        CHAO_SAVE_INFO* freeSlot = reinterpret_cast<CHAO_SAVE_INFO*>(GetFreeChaoSlot());
        auto chao = reward->Value.Chao;

        //Set Dna
        AL_GENE gene = {};
        InitChaoDNA(reinterpret_cast<ChaoDNA*>(&gene));
        gene.EggColor = eCHAO_EGGS::Normal;
        gene.Color[0] = static_cast<eAL_COLOR>(chao.Color);
        gene.Color[1] = static_cast<eAL_COLOR>(chao.Color);
        gene.Multi[0] = chao.Shiny;
        gene.Multi[1] = chao.Shiny;
        gene.NonTex[0] = chao.Tone;
        gene.NonTex[1] = chao.Tone;
        gene.Jewel[0] = static_cast<eCHAO_JEWEL>(chao.Texture);
        gene.Jewel[1] = static_cast<eCHAO_JEWEL>(chao.Texture);

        //Create Chao
        CreateChaoEgg(&gene, (ChaoData*)freeSlot, false, new NJS_VECTOR{100, 100, 100}, 3);

        //Set Type and Garden
        freeSlot->param.Type = chao.Type;
        freeSlot->param.place = (eCHAO_GARDENS)LastChaoArea;

        //Set Name
        strncpy_s((char*)freeSlot + 1572/*CWE Chao name*/, 7, chao.Name, _TRUNCATE);
        
        break;
    }
}

void Give_Mission_Rewards(ChaoMission* mission)
{
    CHAO_PARAM_GC* chao = AL_GBAManagerGetChaoData();

    if (!chao)
    {
        return;
    }

    for (int i = 0; i < mission->Rewards.Amount; i++)
    {
        MissionReward* reward = &mission->Rewards.Rewards[i];
        Give_Reward(reward);
    }

    if (!mission->BonusRewards)
    {
        return;
    }

    int amountOfBonusRequirementsPassed = Check_Mission_Requirements(mission->BonusRequirements, chao);

    for (int i = 0; i < (amountOfBonusRequirementsPassed > mission->BonusRewards->Amount
                             ? mission->BonusRewards->Amount
                             : amountOfBonusRequirementsPassed); i++)
    {
        MissionReward* reward = &mission->BonusRewards->Rewards[i];
        Give_Reward(reward);
    }
}

void Clear_Screen()
{
    AlMsgWarnRemove();
    AlMsgWarnCreate(93, 126.5, 450, 270.0);
    AlMsgWarnOpen();
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

void Load_Mission_Requirement_Text(const MissionRequirements* requirements, std::string initialText,
                                   CHAO_PARAM_GC* chao, size_t amountOfRequirements)
{
    std::string requirementString = std::move(initialText);

    for (size_t i = 0; i < amountOfRequirements; i++)
    {
        MissionRequirement* requirement = &requirements->Requirements[i];
        if (chao)
        {
            bool passed = Check_Mission_Requirement(requirement, chao);
            requirementString += std::string("\n- ") + requirement->RequirementDescription + ": " + (
                passed ? Checkmark : Denymark);
        }
        else
        {
            requirementString += std::string("\n- ") + requirement->RequirementDescription;
        }
    }

    MissionScreenText.push_back(requirementString);
}

void Load_Mission_Reward_Text(const MissionRewards* rewards, std::string initialText, int amountOfRewards,
                              int amountOfRequirementsPassed = -1)
{
    std::string rewardString = std::move(initialText);

    for (int i = 0; i < amountOfRewards; i++)
    {
        MissionReward* reward = &rewards->Rewards[i];
        if (amountOfRequirementsPassed > -1)
        {
            bool passed = amountOfRequirementsPassed >= i;
            rewardString += std::string("\n- ") + reward->RewardDescription + ": " + (passed ? "Unlocked" : "Locked");
        }
        else
        {
            rewardString += std::string("\n- ") + reward->RewardDescription;
        }
    }

    MissionScreenText.emplace_back(rewardString);
}

void Load_Mission_Description_Text(const ChaoMission* mission, int descriptionSize)
{
    MissionScreenText.push_back(
        std::string("---------------------------\n") + mission->Name + "\n---------------------------");

    for (int i = 0; i < descriptionSize; i++)
    {
        std::cout << mission->Description.Lines[i] << std::endl;
        MissionScreenText.emplace_back(mission->Description.Lines[i]);
    }
}

void Load_Mission_Text(int curMission, bool noText)
{
    CurrentLine = 0;
    MissionScreenText.clear();
    Clear_Screen();

    CHAO_PARAM_GC* chao = AL_GBAManagerGetChaoData();
    ChaoMission* mission = &missions.MissionList[curMission];
    MissionDescription* Description = &mission->Description;
    size_t descriptionSize = Description->AmountOfLines;
    size_t amountOfRequirements = mission->Requirements.Amount;
    size_t amountOfRewards = mission->Rewards.Amount;
    size_t amountOfBonusRequirements = mission->BonusRequirements ? mission->BonusRequirements->Amount : 0;
    size_t amountOfBonusRewards = mission->BonusRewards ? mission->BonusRewards->Amount : 0;
    int amountOfBonusRequirementsPassed = -1;
    if (!noText)
    {
        Load_Mission_Description_Text(mission, (int)descriptionSize);
    }

    Load_Mission_Requirement_Text(&mission->Requirements, "\nRequirements:", chao, amountOfRequirements);

    Load_Mission_Reward_Text(&mission->Rewards, "\nRewards:", amountOfRewards);\

    if (amountOfBonusRequirements > 0)
    {
        Load_Mission_Requirement_Text(mission->BonusRequirements, "\nBonus Requirements:", chao,
                                      amountOfBonusRequirements);
    }


    if (amountOfBonusRequirements > 0)
    {
        if (chao)
        {
            amountOfBonusRequirementsPassed = Check_Mission_Requirements(mission->BonusRequirements, chao);
        }

        Load_Mission_Reward_Text(mission->BonusRewards, "\nBonus Rewards:", amountOfBonusRewards,
                                 amountOfBonusRequirementsPassed);
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
    CHAO_PARAM_GC* DataOfChao; // eax
    AL_GBAManagerExecutor_Data* gbaManagerExe;

    v1 = AL_OdekakeMenuMaster_Data_ptr;
    if (AL_OdekakeMenuMaster_Data_ptr)
    {
        switch (AL_OdekakeMenuMaster_Data_ptr->mode)
        {
        case 0x0:
            InitializeMissions();
            LoadMissions();
            LargeTitleBarExecutor_Load(AL_OdekakeMenuMaster_Data_ptr->CurrStage, 650.0, 66.0);
            LoadRingDisplay(40, 400);
            CurrentMission = 0;
            MissionCount = missions.Amount;
            Load_Mission_Arrows();
            AlMsgWarnCreate(93, 126.5, 450, 270.0);
            AlMsgWarnOpen();
            AL_OdeMenuSetMode(0, 0x1);
            break;
        case 0x1:
            Load_Mission_Text(0, false);
            
            AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, MissionScreenText[0].c_str(),
                             TextLanguage == 0);
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
                if ((int)MissionScreenText.size() - 1 > CurrentLine)
                {
                    CurrentLine++;
                    AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14,
                                     MissionScreenText[CurrentLine].c_str(), TextLanguage == 0);

                    if ((int)MissionScreenText.size() - 1 == CurrentLine)
                    {
                        sub_52FB80();
                    }
                    else
                    {
                        sub_543860();
                    }

                    goto LABEL_RETURN;
                }

                CurrentLine = 0;
                Clear_Screen();
                AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, MissionScreenText[0].c_str(),
                                 TextLanguage == 0);
            }
            if ((press & Buttons_Y) != 0)
            {
                Load_Mission_Text(CurrentMission, true);
                AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, MissionScreenText[0].c_str(),
                             TextLanguage == 0);
            }
            if ((press & Buttons_X) != 0)
            {
                DataOfChao = AL_GBAManagerGetChaoData();
                if(DataOfChao)
                {
                    if (RequirementsPassed())
                    {
                        AL_OdeMenuSetMode(0, 0x5);
                    }
                    else
                    {
                        AL_OdeMenuSetMode(0, 0x7);
                        Clear_Screen();
                        AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, "This chao does not yet meet all requirements.", TextLanguage == 0);
                        sub_52FB80();
                    }
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
                    AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, MissionScreenText[0].c_str(),
                             TextLanguage == 0);
                }
            }
            if ((press & Buttons_Right))
            {
                if (CurrentMission < MissionCount - 1)
                {
                    CurrentMission++;
                    Load_Mission_Text(CurrentMission, false);
                    AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, MissionScreenText[0].c_str(),
                             TextLanguage == 0);
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
            break;
        case 0x4:
            if (OdeMenuMasterWork->state == 0)
            {
                AL_OdeMenuSetMode(0, 0xF);
            }
            break;
        case 0x5:
            Give_Mission_Rewards(&missions.MissionList[CurrentMission]);
            AL_OdeMenuSetMode(0, 0x6);
            Clear_Screen();
            AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, "You have received your reward.", TextLanguage == 0);
            AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, "Thanks for helping people with their chao request.", TextLanguage == 0);
            sub_52FB80();
            goto LABEL_RETURN;
        case 0x6:
            if (Al_MSGWarnKinderMessageArray[0].pkindercomessagething14->wordsLeftMaybe > 0)
            {
                goto LABEL_RETURN;
            }
            AL_OdeMenuSetMode(0, 0x9);
            goto LABEL_RETURN;
        case 0x7:
            if (Al_MSGWarnKinderMessageArray[0].pkindercomessagething14->wordsLeftMaybe > 0)
            {
                goto LABEL_RETURN;
            }
            AL_OdeMenuSetMode(0, 0x2);
            Load_Mission_Text(CurrentMission, true);
            AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, MissionScreenText[0].c_str(),
                             TextLanguage == 0);
            goto LABEL_RETURN;
        case 0x9:
            OdeMenuMasterWork->timer++;
            if (OdeMenuMasterWork->timer > 30)
            {
                OdeMenuMasterWork->timer = 0;
                AL_OdeMenuSetMode(0, 0xA);
            }
            break;
        case 0xA:
            DataOfChao = AL_GBAManagerGetChaoData();
            if (DataOfChao)
            {
                memset(DataOfChao, 0, sizeof(CHAO_PARAM_GC));
                gbaManagerExe = AL_GBAManagerExecutor_ptr;
                if (AL_GBAManagerExecutor_ptr)
                {
                    AL_GBAManagerExecutor_ptr->insertedChao = 0;
                    gbaManagerExe->ChaoDataPointer = 0;
                }
            }
            AL_OdeMenuSetMode(0, 0xB);
            goto LABEL_RETURN;
        case 0xB:
            AL_Change_Chao_Area(LastChaoArea);
            AL_OdeMenuSetMode(0, 0x10);
            UnloadMissions();
            goto LABEL_RETURN;
        case 0xF:
            v3 = "chao_r_e.ADX";
            PlayMusic(v3);
            ResetMusic();
            PlaySoundProbably(32787, 0, 0, 0);
            AlMsgWarnRemove();
            DeleteRingDisplay();
            AL_OdekakeMenuMaster_Data_ptr->NextStage = 0;
            AL_OdeMenuChangeStage();
            UnloadMissions();
            goto LABEL_RETURN;
        case 0x10:
            if (ScreenFade2((char)255, 255u, 255u, 255u))
            {
                AL_Change_Chao_Area(LastChaoArea);
            }
            goto LABEL_RETURN;
        default:
        LABEL_RETURN:
            dword_1A557C8 = 0;
            return;
        }
    }
}

// void Print_Mission_Requirements(MissionRequirements* requirements, CHAO_PARAM_GC* chao, size_t amountOfRequirements)
// {
// 	
// 	std::string requirementString;
//
// 	for (size_t i = 0; i < amountOfRequirements; i++)
// 	{
// 		MissionRequirement* requirement = &requirements->Requirements[i];
// 		if (chao)
// 		{
// 			bool passed = Check_Mission_Requirement(requirement, chao);
// 			requirementString = std::string("- ") + requirement->RequirementDescription + ": " + (passed ? Checkmark : Denymark);
// 		}
// 		else
// 		{
// 			requirementString = std::string("- ") + requirement->RequirementDescription;
// 		}
// 		AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, requirementString.c_str(), TextLanguage == 0);
// 	}
// }
//
// void Print_Mission_Rewards(MissionRewards* rewards, int amountOfRewards, int amountOfRequirementsPassed = -1)
// {
// 	std::string rewardString;
//
// 	for (int i = 0; i < amountOfRewards; i++)
// 	{
// 		MissionReward* reward = &rewards->Rewards[i];
// 		if (amountOfRequirementsPassed > -1)
// 		{
// 			bool passed = amountOfRequirementsPassed >= i;
// 			rewardString = std::string("- ") + reward->RewardDescription + ": " + (passed ? "Unlocked" : "Locked");
// 		}
// 		else 
// 		{
// 			rewardString = std::string("- ") + reward->RewardDescription;
// 		}
// 		AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, rewardString.c_str(), TextLanguage == 0);
// 	}
// }
//
// void Print_Mission_Text(ChaoMission* mission, int descriptionSize)
// {
//     MissionTitle = std::string("---------------------------\n") + mission->Name + "\n---------------------------";
// 	AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, _strdup(MissionTitle.c_str()), TextLanguage == 0);
//
// 	for (int i = 0; i < descriptionSize; i++)
// 	{
// 		std::cout << mission->Description.Lines[i] << std::endl;
// 		AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, mission->Description.Lines[i], TextLanguage == 0);
//
// 		sub_543860();
// 	}
// }
//
// void Load_Mission_Text(int curMission, bool noText)
// {
// 	AlMsgWarnRemove();
// 	AlMsgWarnCreate(93, 126.5, 450, 270.0);
// 	AlMsgWarnOpen();
//
// 	CHAO_PARAM_GC* chao = AL_GBAManagerGetChaoData();
// 	ChaoMission* mission = &missions.MissionList[curMission];
// 	MissionDescription* Description = &mission->Description;
// 	size_t descriptionSize = Description->AmountOfLines;
// 	size_t amountOfRequirements = mission->Requirements.Amount;
// 	size_t amountOfRewards = mission->Rewards.Amount;
// 	size_t amountOfBonusRequirements = mission->BonusRequirements ? mission->BonusRequirements->Amount : 0;
// 	size_t amountOfBonusRewards = mission->BonusRewards ? mission->BonusRewards->Amount : 0;
// 	int amountOfBonusRequirementsPassed = -1;
// 	if (!noText)
// 	{
// 		Print_Mission_Text(mission, descriptionSize);
// 	}
//
// 	AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, "\nRequirements:", TextLanguage == 0);
// 	Print_Mission_Requirements(&mission->Requirements, chao, amountOfRequirements);
//
// 	sub_543860();
//
// 	AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, "\nRewards:", TextLanguage == 0);
// 	Print_Mission_Rewards(&mission->Rewards, amountOfRewards);
//
// 	sub_543860();
//
// 	AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, "\nBonus Requirements:", TextLanguage == 0);
// 	Print_Mission_Requirements(mission->BonusRequirements, chao, amountOfBonusRequirements);
//
// 	sub_543860();
//
// 	if (chao)
// 	{
// 		amountOfBonusRequirementsPassed = Check_Mission_Requirements(mission->BonusRequirements, chao);
// 	}
//
// 	AlMsgWinAddLineC(Al_MSGWarnKinderMessageArray[0].pkindercomessagething14, "\nBonus Rewards:", TextLanguage == 0);
// 	Print_Mission_Rewards(mission->BonusRewards, amountOfBonusRewards, amountOfBonusRequirementsPassed);
//
// 	sub_52FB80();
// }

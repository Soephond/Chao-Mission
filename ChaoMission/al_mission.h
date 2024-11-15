#pragma once

#include "sa2modloader.h"

#include <functional>

#include "cwe_api.h"
#include "al_odekake.h"
#include "al_ui.h"
#include "al_chao_home.h"
#include "chao_data.h"
#include "al_mission_structs.h"

EXTERN_C_START
void Display_Mission_Arrows(ObjectMaster* a1);
void Load_Mission_Text(int curMission, bool noText);
void Mission_Arrow_Executor(ObjectMaster* a1);
void Load_Mission_Arrows();
void Handle_Mission_Menu(ODE_MENU_MASTER_WORK* OdeMenuMasterWork);
bool Check_Mission_Requirement(MissionRequirement* requirement, CHAO_PARAM_GC* chao);
bool RequirementsPassed();
void Give_Mission_Reward(MissionReward* Reward);
EXTERN_C_END

extern ChaoHudThingB MissionMenu[];
extern ChaoMissions missions;
extern int CurrentMission;
extern int MissionCount;

static CWE_API_ODEKAKE_ENTRY MissionEntry = { Handle_Mission_Menu, nullptr, ODE_FLAGS_NONE, &MissionMenu[2],&MissionMenu[0],&MissionMenu[1],&MissionMenu[3],&MissionMenu[4], 1.0, 1.0, 0.5, 0.5 };
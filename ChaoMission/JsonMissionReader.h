#pragma once

#include "al_mission_structs.h"
#include "file_helper.h"


ChaoMission LoadChaoMissionFromJson(jsonDocument& missionJson);
std::string GetMissionNameFromJson(jsonValue& missionJson);
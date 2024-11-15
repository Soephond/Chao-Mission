#pragma once

#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "al_mission_structs.h"

#define MissionPath "./mods/C.H.A.O/Missions"
#define MissionErrorPath MissionPath "/Errors"
#define MissionClearPath MissionPath "/Cleared"
#define MissionBacklogPath MissionPath "/Backlog"
#define MissionActivePath MissionPath "/InProgress"

extern int AmountOfMissions;
extern int MaxAmountOfMissions;

int GetAmountOfActiveMissions();
std::vector<ChaoMission> GetActiveMissions();
void InitializeMissions(int maxAmountOfMissions = MaxAmountOfMissions);
void SetNewMission();
void ClearMission(std::string missionName);
std::filesystem::path GetMissionFileByName(std::string missionName);
void RemoveMission(std::string missionName);
void RemoveMission(std::filesystem::path missionFile);
bool IsValidMission(std::filesystem::path missionFile);
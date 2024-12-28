#include <filesystem>

#include "MissionFileHandler.h"
#include "JsonMissionReader.h"
#include "file_helper.h"


int __readonly AmountOfMissions = GetAmountOfActiveMissions();
int MaxAmountOfMissions = 0;

int GetAmountOfActiveMissions()
{
    int amount = GetFileCount(MissionActivePath);

    return amount > MaxAmountOfMissions ? MaxAmountOfMissions : amount;
}

std::vector<ChaoMission> GetActiveMissions()
{
    int amount = GetAmountOfActiveMissions();

    if (amount == 0)
    {
        std::cout << "No active missions" << std::endl;
        return std::vector<ChaoMission>();
    }

	int i = 0;
    std::vector<ChaoMission> missions(amount);

    for (const auto& entry : GetJsonFiles(MissionActivePath, amount))
    {
	    try
	    {
	    	jsonDocument jsonData = ReadJsonFromFile(entry.string());
	    	missions[i] = LoadChaoMissionFromJson(jsonData);
	    	i++;
	    }
	    catch (...)
	    {
	    	RemoveMission(entry);
	    	amount--;
	    }
        
    }

	return missions;
}

void InitializeMissions(int maxAmountOfMissions)
{
    MaxAmountOfMissions = maxAmountOfMissions;

    //Try and add new missions if there is space
    while (AmountOfMissions < MaxAmountOfMissions && GetFileCount(MissionBacklogPath) > 0)
    {
        auto missionFiles = GetRandomJsonFiles(MissionBacklogPath, MaxAmountOfMissions - AmountOfMissions);

        for (const auto& file : missionFiles)
        {
	        	if(IsValidMission(file))
	        	{
	        		std::filesystem::path newPath = MissionActivePath / file.filename();
	        		std::filesystem::rename(file, newPath);
	        	}
		        else
		        {
		        	RemoveMission(file);
		        }
        }
    }
}

void SetNewMission()
{
	bool validMission = false;
	std::filesystem::path missionFile;

	while (validMission == false && GetFileCount(MissionBacklogPath) > 0)
	{
		missionFile = GetRandomJsonFiles(MissionBacklogPath, 1).front();
		validMission = IsValidMission(missionFile);
	}

	if (missionFile.empty())
	{
		std::cout << "No missions available" << std::endl;
		return;
	}

	//Add mission to active missions
	std::filesystem::path newPath = MissionActivePath / missionFile.filename();
	std::filesystem::rename(missionFile, newPath);
}

std::filesystem::path GetMissionFileByName(const std::string &missionName)
{
	//loop through the 4 directories and find the mission file
	std::filesystem::path path;

	auto dirs = { MissionActivePath, MissionBacklogPath, MissionClearPath, MissionErrorPath };

	for (const auto& directory: dirs)
	{
		for (const auto& entry : std::filesystem::directory_iterator(directory))
		{
			try
			{
				jsonDocument jsonData = ReadJsonFromFile(entry.path().string());
				std::string missionNameJson = GetMissionNameFromJson(jsonData);
				
				if (missionName == missionNameJson)
				{
					path = entry.path();
					break;
				}
			}
			catch (...)
			{
				RemoveMission(entry.path());
			}
			
		}
	}

	return path;
}

void ClearMission(const std::string &missionName)
{
	auto missionFile = GetMissionFileByName(missionName);

	if (missionFile.empty())
	{
		std::cout << "No missions available" << std::endl;
		return;
	}

	//Add mission to cleared missions
	std::filesystem::path newPath = MissionClearPath / missionFile.filename();
	std::filesystem::rename(missionFile, newPath);
}

void RemoveMission(std::string missionName)
{
	auto missionFile = GetMissionFileByName(missionName);

	if (missionFile.empty())
	{
		std::cout << "No missions available" << std::endl;
		return;
	}

	RemoveMission(missionFile);
}

void RemoveMission(std::filesystem::path missionFile)
{
	//Add mission to removed missions
	std::filesystem::path newPath = MissionErrorPath / missionFile.filename();
	std::filesystem::rename(missionFile, newPath);
}

bool IsValidMission(std::filesystem::path missionFile)
{
	try
	{
		jsonDocument jsonData = ReadJsonFromFile(missionFile.string());

		if (jsonData.IsNull())
		{
			return false;
		}
		
		LoadChaoMissionFromJson(jsonData);
	}
	catch (...)
	{
		return false;
	}
	
	return true;
}
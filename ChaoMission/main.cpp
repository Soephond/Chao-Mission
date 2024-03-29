﻿#pragma once
#include "pch.h"

#include <fstream>
#include <iostream>
#include <array>
#include <format>

#include "FunctionHook.h"
#include "cwe_api.h"

#include "al_odekake.h"
#include "al_blackmarket.h"
#include "al_mission.h"
#include "al_helper.h"
#include "al_ui.h"
#include "al_tex.h"
#include "al_chao_home.h"

using namespace std;

FunctionPointer(void, sub_5ACBB0, (float a1, float a2), 0x5ACBB0);
FastcallFunctionPointer(void, sub_443030, (int a1), 0x443030);

bool CWE_Loaded = false;
const HelperFunctions* HelperFunctionsGlobal;

FastcallFunctionHook<void, ODE_MENU_MASTER_WORK*> Call_Goodbye_Menu_hk(0x5A6F50);


extern "C" {


	//registering data functions. - Needs to exist.
	void (*RegisterDataFunc)(void* ptr);


	//main CWE Load function -- Important stuff like adding your CWE mod goes here
	void CWELoad(CWE_REGAPI* cwe_api, ObjectMaster* tp)
	{
		LoadChaoTexlist("ChaoHomeTexList", AL_ODE_CHAO_TEXLIST, 1);
		CWE_Loaded = HelperFunctionsGlobal->Mods->find("CWE");
		if (CWE_Loaded) //If CWE is loaded
		{
			Call_Goodbye_Menu_hk.Hook(Handle_Adoption_Screen);
			cwe_api->AddOdekakeMenu(MissionEntry);
		}
		else
		{
			Call_Goodbye_Menu_hk.Hook(Handle_CHAO_Menu);
		}
	}

	// Optional.
	// This function runs code one time when the game starts up. Great for loading assets and setting things up.
	__declspec(dllexport) void __cdecl Init(const char* path, const HelperFunctions& helperFunctions)
	{
		HelperFunctionsGlobal = &helperFunctions;

		HMODULE h = GetModuleHandle(L"CWE");

		std::string pathStr = std::string(path) + "\\";

		RegisterDataFunc = (void (*)(void* ptr))GetProcAddress(h, "RegisterDataFunc");
		RegisterDataFunc(CWELoad);
	}

	// Optional.
	// This function runs code on every frame of the game, INCLUDING being in menus, cutscenes, etc.
	// It is recommended to test for game state so that you only run code at a specific time.
	__declspec(dllexport) void __cdecl OnFrame()
	{
	}

	// Optional.
	// This function runs code every time the player inputs. Good for adding custom inputs / overriding events.
	__declspec(dllexport) void __cdecl OnInput()
	{
	}

	// Optional.
	// This function runs while the game processes input.
	__declspec(dllexport) void __cdecl OnControl()
	{
	}


	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer }; // This is needed for the Mod Loader to recognize the DLL.
}

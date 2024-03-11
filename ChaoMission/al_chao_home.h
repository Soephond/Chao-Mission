#pragma once
#include "SA2Structs.h"
#include "FunctionHook.h"
#include "al_odekake.h"
#include "al_blackmarket.h"
#include "al_msg.h"
#include "al_helper.h"
#include "al_ui.h"

extern NJS_TEXNAME AL_ODE_CHAO_TEXNAME[1];
extern NJS_TEXLIST AL_ODE_CHAO_TEXLIST[];

int CalculateReward(ChaoData* chao);
void __fastcall Handle_CHAO_Menu(ODE_MENU_MASTER_WORK* OdeMenuMasterWork);
void __fastcall Handle_Adoption_Screen(ODE_MENU_MASTER_WORK* OdeMenuMasterWork);
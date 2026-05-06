# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What This Is

**C.H.A.O. (Chao Mission)** is a Sonic Adventure 2 mod that adds a mission system to the Chao World. Players can accept "requests" for specific Chao, complete them by presenting a Chao that meets all requirements, and receive in-game rewards. The mod integrates with the **Chao World Extended (CWE)** mod loader as an Odekake (field trip) menu entry.

The output is a Win32 DLL (`ChaoMission.dll`) loaded by the SA2 Mod Loader.

## Build

Open `ChaoMission.sln` in Visual Studio 2022 and build the **Release|Win32** configuration. The output DLL is placed directly into `$(SA2_PATH)\mods\C.H.A.O.` (requires `SA2_PATH` environment variable pointing to your SA2 install).

Dependencies managed via NuGet:
- `nlohmann.json 3.11.2` — used via `rapidjson` wrappers in `file_helper.h`
- `magic_enum` — used in `JsonMissionReader.cpp` for enum-to-string serialization in JSON parsing

There is no test suite; correctness must be verified in-game.

## Architecture

### Entry Point (`main.cpp`)
Exports the standard SA2 Mod Loader symbols: `Init`, `OnFrame`, `OnInput`, `OnControl`, `OnExit`. On `Init`, reads `config.ini` for `AmountActiveMissions`, then hooks into CWE's `RegisterDataFunc`. In `CWELoad`, registers `MissionEntry` as an Odekake menu if CWE is present, otherwise hooks the Chao adoption/goodbye screen directly.

### Mission Lifecycle (`MissionFileHandler.cpp`)
Missions are JSON files that move between four directories under `./mods/C.H.A.O/Missions/`:
- `Backlog/` — available but not yet active
- `InProgress/` — currently presented to the player (capped by `AmountActiveMissions`)
- `Cleared/` — successfully completed missions
- `Errors/` — missions that failed to parse (quarantined)

`InitializeMissions()` promotes missions from Backlog → InProgress on menu open. `ClearMission()` moves a completed mission to Cleared. Invalid missions are automatically quarantined to Errors.

### Mission Data Model (`al_mission_structs.h`)
- `ChaoMission` — top-level struct: name, description lines, requirements, rewards, and optional bonus requirements/rewards
- `MissionRequirement` — a `REQUIREMENT_TYPE` + a linked list of `ValueCheckPoint` (the list represents OR logic: any node passing means the requirement passes; within a node, all checks are ANDed)
- `MissionReward` — typed reward: rings, fruit, animals, seeds, eggs, hats, or a fully-specified Chao

### JSON Parsing (`JsonMissionReader.cpp`)
`LoadChaoMissionFromJson()` parses a `rapidjson` document into a `ChaoMission`. All enum values in JSON can be either the string name (via `magic_enum`) or the raw integer. The `"Checks"` array in each requirement is an array-of-arrays: outer array = OR branches, inner array = AND checks within a branch.

### Mission UI (`al_mission.cpp`)
`Handle_Mission_Menu()` is the main state machine (modes 0x0–0x10) driving the in-game menu. It uses the SA2 text/message system (`AlMsgWarn*`, `AlMsgWin*`) for all text display. Navigation uses D-pad left/right to switch missions, A to page through text, X to submit the currently held Chao, B to exit.

### Requirement Checking (`al_mission.cpp`)
`Check_Mission_Requirement()` walks the `ValueCheckPoint` linked list and evaluates each `ValueCheck` against the live `CHAO_PARAM_GC*` data obtained from `AL_GBAManagerGetChaoData()`. Requirement categories: Type, Stat, Appearance, Ability, Toy, Race, Karate, Classes, Lifetime, Bond.

### SA2 Interop Headers
- `SA2Structs.h`, `SA2Variables.h`, `SA2Functions.h`, `SA2Enums.h` — reverse-engineered game structures and function pointers at hard-coded memory addresses
- `cwe_api.h` — CWE mod API for registering Odekake menu entries
- `MemAccess.h`, `FunctionHook.h`, `Trampoline.h` — SA2 Mod Loader utilities for patching game memory and hooking functions

## Adding a New Mission

Create a JSON file in `C.H.A.O/Missions/Backlog/`. Required top-level keys: `Name` (string), `Description` (string array), `Requirements` (array), `Rewards` (array). Optional: `Bonus Requirements`, `Bonus Rewards`. See `ChaoFarmer.json` for a complete example with OR-branched requirements and all reward types.

## Knowledge Sources

- **Tehfusion Chao value info** - https://chao.tehfusion.co.uk/chao-hacking/ - main reference for all value information for chao data and the like. Consult this when needing clarification regarding chao data.
- **Chao Island Wiki** — https://chao-island.com/wiki/Chao_Wiki and https://chao-island.com/wiki/Chao_World_Extended — authoritative reference for all Chao-related game mechanics, stat names, fruit types, animal parts, egg types, race medals, karate ranks, and anything else Chao-specific. Consult this when working on mission requirements, rewards, or any game mechanic terminology

## Docs

CI deploys MkDocs documentation to GitHub Pages on pushes to `master`. Run `pip install mkdocs-material` and `mkdocs serve` to preview locally.

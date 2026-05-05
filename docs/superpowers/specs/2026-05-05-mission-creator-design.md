# C.H.A.O. Mission Creator — Design Spec

**Date:** 2026-05-05  
**Branch:** feature/Mission_Creator  
**Status:** Approved

---

## Overview

A Windows desktop GUI application for creating and editing C.H.A.O. mission JSON files. Target audience is non-technical modders who are not familiar with JSON. Users can create missions from scratch or load existing ones via drag-and-drop. Output is a JSON file compatible with the C.H.A.O. mod's mission schema.

---

## Architecture

**Stack:** .NET MAUI Blazor Hybrid, Windows-only (`net10.0-windows10.0.19041.0`). The app runs as a native Windows executable hosting a Blazor WebView inside MAUI's `MainPage`. Android, iOS, and MacCatalyst targets are dropped.

**Key layers:**

| Layer | Responsibility |
|---|---|
| `Models/` | C# editor-facing data classes (`MissionEditorModel`, `RequirementModel`, `RewardModel`, etc.) |
| `Services/MissionSerializer.cs` | Converts `MissionEditorModel` ↔ JSON. The only class aware of the JSON schema. |
| `Services/MissionFileService.cs` | File I/O: drag-drop load, Save As dialog, save to mod Backlog folder, bonus mismatch check |
| `Services/AppSettingsService.cs` | Persists SA2 install path in `LocalApplicationData` |
| `Components/` | Blazor Razor components for the UI |

`MissionSerializer` being the sole JSON-aware class is the key extensibility decision: a future live JSON preview panel is a one-liner — bind a `<pre>` to `Serializer.Serialize(model)`.

---

## Layout

Single screen, split horizontally into a fixed left panel (~380px) and a flexible right panel.

```
┌─────────────────────────────────────────────────────────────────────┐
│  [≡ C.H.A.O. Mission Creator]                          [⚙ Settings] │
├──────────────────────────┬──────────────────────────────────────────┤
│  MISSION INFO            │  [Standard]  [Bonus]                     │
│                          ├─────────────────────┬────────────────────┤
│  Name: [____________]    │  REQUIREMENTS       │  REWARDS           │
│                          │  [+ Add Requirement]│  [+ Add Reward]    │
│  Description:            ├─────────────────────┼────────────────────┤
│  [line 1          ] [x]  │ ▲ (scrollable)      │ ▲ (scrollable)     │
│  [line 2          ] [x]  │  ┌── Req 1 ───────┐ │  ┌── Reward 1 ──┐  │
│  [+ Add line]            │  │ ...            │ │  └──────────────┘  │
│                          │  └────────────────┘ │                    │
│  ┌──────────────────┐    │ ▼                   │ ▼                  │
│  │  Drop .json here  │    │                     │                    │
│  │  or click to open │    │                     │                    │
│  └──────────────────┘    │                     │                    │
├──────────────────────────┴─────────────────────┴────────────────────┤
│  [Save to Mod Folder]   [Save As...]   Status: Ready                │
└─────────────────────────────────────────────────────────────────────┘
```

- **Left panel:** mission name, description lines (each removable, add line button), drag-and-drop zone. Drop zone disappears once a mission is loaded, replaced by a small "Load different file" link.
- **Right panel:** two tabs — **Standard** and **Bonus**. Each tab is a two-column split where Requirements (left) and Rewards (right) scroll independently. Column headers and add buttons are sticky.
- **Bottom bar:** always-visible sticky footer with Save to Mod Folder, Save As, and a status text area.
- **Future extension:** a third panel column for a live JSON preview can be added without structural changes — just add a CSS grid column and a toggle button in the header.

---

## Tabs

### Standard Tab
- **Left column — Requirements:** list of `RequirementCard` components, `[+ Add Requirement]` sticky header button.
- **Right column — Rewards:** list of `RewardCard` components, `[+ Add Reward]` sticky header button.

### Bonus Tab
- Same two-column layout as Standard, reusing the exact same card components.
- **Left column — Bonus Requirements**, **Right column — Bonus Rewards.**
- Single `[+ Add Bonus]` button at the bottom of the tab (adds one entry to each list independently).
- No pairing enforced in the model — mismatch is caught at save time (see Save Behaviour).

---

## Requirement UI (OR/AND Logic)

Each `RequirementCard` shows:
- **Requirement type** dropdown (TypeRequirement, StatRequirement, AppearanceRequirement, …)
- **Description** text field
- One or more **OR branches**, each containing one or more **AND conditions**
- `[+ Add condition]` within a branch adds an AND condition
- `[+ Add OR branch]` below the last branch adds an alternative branch, visually separated by an "OR" label
- OR branches and individual conditions each have an `[x]` remove button

---

## Data Model

```
MissionEditorModel
  ├── string Name
  ├── List<string> DescriptionLines
  ├── List<RequirementModel> Requirements
  ├── List<RewardModel> Rewards
  ├── List<RequirementModel> BonusRequirements
  └── List<RewardModel> BonusRewards

RequirementModel
  ├── REQUIREMENT_TYPE Type
  ├── string Description
  └── List<List<CheckModel>> Checks    // outer = OR branches, inner = AND conditions

CheckModel
  ├── string Type                      // e.g. ChaoTypeCheck, LevelCheck, AnimalArmsCheck
  └── Dictionary<string, object> Properties   // type-specific fields (Value, Skill, MinValue…)

RewardModel
  ├── REWARD_TYPE Type                 // RingReward, FruitReward, AnimalReward,
  │                                   //   SeedReward, EggReward, HatReward, ChaoReward
  ├── int Amount
  ├── string Description
  └── RewardValue? Value              // null for RingReward

EnumRewardValue : RewardValue         // Fruit, Animal, Seed, Egg, Hat
  ├── string Key                      // enum name e.g. "Al_Animal_Bear"
  └── int IntValue                    // numeric value e.g. 8

ChaoRewardValue : RewardValue         // ChaoReward only
  ├── int ChaoType
  ├── int Color
  ├── string Texture
  ├── int Tone
  ├── string Shiny
  └── string Name
```

For `EnumRewardValue`, the UI shows a dropdown of known named values — the user picks a name and the int fills in automatically.

---

## Component Tree

```
Pages/
  MissionEditor.razor                  ← root page, owns MissionEditorModel state

Components/
  MissionInfo.razor                    ← left panel: name, description, drop zone
  EditorTabs.razor                     ← Standard / Bonus tab switcher

  Standard/
    StandardTab.razor                  ← two-column split
    RequirementPanel.razor             ← scrollable column, sticky header
    RequirementCard.razor              ← one requirement with OR/AND UI
    OrBranch.razor                     ← one OR branch with condition list
    CheckRow.razor                     ← one condition: type dropdown + dynamic fields
    RewardPanel.razor                  ← scrollable column, sticky header
    RewardCard.razor                   ← one reward: type, amount, description, value picker

  Bonus/
    BonusTab.razor                     ← reuses RequirementPanel/Card + RewardPanel/Card
                                       ← [+ Add Bonus] button adds one to each list

  Shared/
    SaveBar.razor                      ← sticky footer
    DropZone.razor                     ← drag-drop target inside MissionInfo
    ConfirmDialog.razor                ← bonus mismatch warning
```

---

## Services

### AppSettingsService
Persists the SA2 install path in `LocalApplicationData`. On first launch the Settings panel (opened via the gear icon) prompts the user to browse for the path.

### MissionFileService
- **Load (drag-drop / open):** reads the `.json` file, calls `MissionSerializer.Deserialize()`, populates the editor.
- **Save As:** opens a `FileSavePicker`, serializes, writes.
- **Save to Mod Folder:** writes to `{SA2_PATH}\mods\C.H.A.O\Missions\Backlog\{Name}.json`. If SA2 path is not configured, opens the Settings panel first.
- **Bonus mismatch check:** runs before either save path.
- **Status text:** the save bar status area shows the result of the last save operation (e.g. "Saved to Backlog", "Saved to C:\…\mission.json", or an error message).

### MissionSerializer
- `Deserialize(string json) → MissionEditorModel`
- `Serialize(MissionEditorModel) → string` — always outputs enum string names (e.g. `"Al_Animal_Bear"`); handles both string and int enum input on deserialize.

---

## Save Behaviour

### Bonus count mismatch
Before saving, if `BonusRequirements.Count ≠ BonusRewards.Count`, show `ConfirmDialog`:

**When bonus requirements > bonus rewards:**
> "You have more bonus requirements than bonus rewards. Players who clear all [bonusRequirements.Count] bonus requirements will only receive [bonusRewards.Count] bonus rewards — the extra cleared requirements won't grant anything. Make sure the number of bonus requirements and bonus rewards are equal. Save anyway?"

**When bonus rewards > bonus requirements:**
> "Only [bonusRequirements.Count] of the [bonusRewards.Count] bonus rewards can be given out — there aren't enough bonus requirements to unlock the rest. Make sure the number of bonus requirements and bonus rewards are equal. Save anyway?"

Both dialogs show **Save anyway** and **Go back** buttons.

---

## Settings Panel

Opened via the gear icon in the header. Contains:

- **SA2 Install Path** — browse button to set/change the path. Required for "Save to Mod Folder".
- **Enable all checks** — toggle (off by default). Tooltip: *"Enables advanced checks such as character fear, distance, and meet count. Use with caution — these values are less documented."* When off, only the most common check variants are shown per requirement type (e.g. only CharacterLikeCheck for BondRequirement).

---

## Requirement Check UI — Per Type

Each check row is rendered by `CheckRow.razor`. The type dropdown at the top of the row determines which fields appear below it. All checks support a **range mode selector** (Exact / Min / Max / Range) and an **Inverted** toggle unless noted otherwise.

### TypeRequirement
Check type dropdown with options:
- **Chao Type** → enum dropdown (ChaoType: Child, Good, Bad, Neutral/Hero/Dark × Normal/Swim/Fly/Run/Power/Chaos, Tails, Knuckles, Amy)
- **Swim ↔ Fly** → slider −1.0 (Swim) → +1.0 (Fly) with live label (e.g. "Leaning Fly")
- **Run ↔ Power** → slider −1.0 (Run) → +1.0 (Power) with live label
- **Alignment** → slider −1.0 (Dark) → +1.0 (Hero) with live label (e.g. "Neutral")
- **Magnitude** → slider 0.0 → 1.0 with live label (e.g. "Full")

### AppearanceRequirement
Check type dropdown with options:
- **Color** → ChaoColor dropdown
- **Tone** → ChaoTone dropdown (MonoTone, TwoTone)
- **Shiny** → ChaoShiny dropdown (None, Bright, Shiny)
- **Texture** → SA2BTexture dropdown (None → Moon; CWE textures added when list is supplied)
- **Arms / Ears / Forehead / Horn / Legs / Tail / Wings / Face** → Al_Animal dropdown filtered to animals that have the selected body part, grouped: SA2B | SADX via CWE | New CWE

### StatRequirement
Check type dropdown:
- **Level** → Skill dropdown (Swim/Fly/Run/Power/Stamina/Luck/Intelligence) + slider 0–99
- **Grade** → Skill dropdown + ChaoGrade dropdown (E/D/C/B/A/S/X)
- **Points** → Skill dropdown + slider 0–4000 (Luck/Intelligence cap: 4000; others: 3266)
- **Happiness** → slider 0–(max short); no skill needed

### AbilityRequirement
- **Specific Ability (FlagCheck)** → grouped multi-select showing ability name + animal hint:
  - Swim: Belly Slide (Penguin), Roll Around (Seal), Backstroke (Otter)
  - Run: Hop Around (Rabbit), Wash Face (Cheetah), Dash (Warthog)
  - Power: Roar (Bear), Sharpen Nails (Tiger), Pound Chest (Gorilla)
  - Fly: Strut (Peacock), Whistle (Parrot), Sit-ups (Condor)
  - Other: Fart (Skunk), Somersault (Sheep), Hum (Raccoon), Fiery Chao Ball (Half Fish), Lose All Parts (Skeleton Dog), Lose Legs (Bat), Breathe Fire (Dragon), Buck Feet (Unicorn), Wag Tail (Phoenix)
  - Selected abilities OR'd into `__int16` bitmask
- **Total Abilities (TotalCheck)** → slider 0–21

### ToyRequirement
- **Specific Toy (FlagCheck)** → multi-select: Rattle, Car, Picture Book, Sonic Doll, Broomstick, Pogo Stick, Crayons, Bubble Wand, Shovel, Watering Can
- **Total Toys (TotalCheck)** → slider 0–10

### ClassesRequirement
- **Specific Class (FlagCheck)** → grouped multi-select:
  - Drawing: Level 1–5
  - Dance: Shake, Spin, Step, Go-Go
  - Other: Exercise
  - Song: Level 1–5
  - Instruments: Bell, Castanets, Cymbals, Drum, Flute, Maracas, Trumpet, Tambourine
- **Total Classes (TotalCheck)** → slider 0–23

### RaceRequirement
- **Specific Medal (FlagCheck)** → multi-select: Challenge, Beginner, Hero, Dark, Aquamarine, Topaz, Peridot, Garnet, Onyx, Diamond, Pearl, Amethyst, Emerald, Ruby, Sapphire
- **Total Medals (TotalCheck)** → slider 0–15

### KarateRequirement
- **Rank (KarateRankCheck)** → dropdown: No Rank, 10th–1st Kyu, 1st–9th Degree, Grand Master
- **Matches Won (AmountWonCheck)** → number input

### LifetimeRequirement
- **Age (AgeCheck)** → number input
- **Reincarnations (ReincarnationsCheck)** → number input

### BondRequirement
- **Character** dropdown always shown first: Sonic, Shadow, Tails, Eggman, Knuckles, Rouge
- **Like (CharacterLikeCheck)** → slider −100 (Hates) → +100 (Loves) with live label — always available
- **Fear / Distance / Meet** → hidden unless "Enable all checks" is on in Settings; number inputs

---

## Reward Card UI — Per Type

Each `RewardCard` has: **Type** dropdown, **Amount** number input, **Description** text field, and a type-specific **Value** picker.

| Type | Value picker |
|---|---|
| RingReward | No value field |
| FruitReward | SA2BFruit dropdown (flat, 24 entries) |
| AnimalReward | Al_Animal grouped dropdown: SA2B (21) \| SADX via CWE (15: Mole, Koala, CWE variants) \| New CWE (3: Bee, Ladybug, Scorpion) |
| SeedReward | ChaoSeed dropdown: Strong, Tasty, Hero, Dark, Round, Triangle, Square |
| EggReward | eCHAO_EGGS grouped dropdown: Normal \| MonoTone (13) \| TwoTone (13) \| Shiny Normal \| Shiny MonoTone (13) \| Shiny TwoTone (13) \| Jewel/Special (Gold, Silver, Ruby, Sapphire, Emerald, Amethyst, Aquamarine, Garnet, Onyx, Peridot, Topaz, Pearl, Metal1, Metal2, Glass) |
| HatReward | SA2BHat grouped dropdown: Named Hats (Pumpkin, Skull, Apple, Bucket, Empty Can, Cardboard Box, Flower Pot, Paper Bag, Pan, Stump, Watermelon, Red/Blue/Black Wool Beanie, Pacifier) \| EggShell variants (MonoTone, TwoTone, Shiny MonoTone, Shiny TwoTone, Jewel) |
| ChaoReward | Expanded inline fields: Name (text, max 12 chars), ChaoType dropdown, Color dropdown, Texture dropdown, Tone dropdown, Shiny dropdown |

---

## Future Extensions (not in scope now)

- **Live JSON preview panel:** add a third CSS grid column, bind a `<pre>` to `MissionSerializer.Serialize(model)`. No other changes needed.
- **Mission list / manager view:** browse Backlog / InProgress / Cleared folders.
- **Validation:** highlight incomplete requirements or rewards before saving.
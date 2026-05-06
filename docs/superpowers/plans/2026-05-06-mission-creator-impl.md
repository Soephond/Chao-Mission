# C.H.A.O. Mission Creator — Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Build the C.H.A.O. Mission Creator MAUI Blazor Hybrid app from blank template to a fully functional mission JSON editor.

**Architecture:** Single-page MAUI Blazor Hybrid Windows app. `MissionEditorModel` is the single source of truth for all UI state. `MissionSerializer` is the only class that touches JSON. File I/O goes through `MissionFileService`.

**Tech Stack:** .NET 10 MAUI Blazor Hybrid, Windows-only (`net10.0-windows10.0.19041.0`), `System.Text.Json`, `Microsoft.Win32` for file dialogs.

---

## File Map

| File | Action | Purpose |
|---|---|---|
| `C.H.A.O. Mission Creator.csproj` | Modify | Windows-only target |
| `MauiProgram.cs` | Modify | Register services |
| `Components/_Imports.razor` | Modify | Add model/service usings |
| `Components/Layout/MainLayout.razor` | Replace | App shell layout |
| `Components/Pages/MissionEditor.razor` | Create | Root page, owns state |
| `Components/MissionInfo.razor` | Create | Left panel |
| `Components/EditorTabs.razor` | Create | Tab switcher |
| `Components/Standard/StandardTab.razor` | Create | Two-column req/reward layout |
| `Components/Standard/RequirementPanel.razor` | Create | Scrollable req column |
| `Components/Standard/RequirementCard.razor` | Create | Single requirement UI |
| `Components/Standard/OrBranch.razor` | Create | One OR branch |
| `Components/Standard/CheckRow.razor` | Create | One condition (all 11 types) |
| `Components/Standard/RewardPanel.razor` | Create | Scrollable reward column |
| `Components/Standard/RewardCard.razor` | Create | Single reward UI (all 7 types) |
| `Components/Bonus/BonusTab.razor` | Create | Bonus tab (reuses panels) |
| `Components/Shared/SaveBar.razor` | Create | Sticky footer |
| `Components/Shared/DropZone.razor` | Create | Drag-drop target |
| `Components/Shared/ConfirmDialog.razor` | Create | Mismatch warning dialog |
| `Components/Shared/SettingsPanel.razor` | Create | SA2 path + toggles |
| `Models/GameEnums.cs` | Create | All C# enums |
| `Models/MissionEditorModel.cs` | Create | Top-level model |
| `Models/RequirementModel.cs` | Create | Requirement + Check models |
| `Models/RewardModel.cs` | Create | Reward model |
| `Models/ColorReferenceData.cs` | Create | Color group records |
| `Services/ColorReferenceService.cs` | Create | Loads color JSON |
| `Services/AppSettingsService.cs` | Create | Persists SA2 path |
| `Services/MissionSerializer.cs` | Create | JSON serialize/deserialize |
| `Services/MissionFileService.cs` | Create | File I/O |
| `wwwroot/css/app.css` | Replace | App styles |
| `wwwroot/js/dropzone.js` | Create | Drag-drop JS interop |

---

## Task 1: Project Setup

**Files:**
- Modify: `C.H.A.O. Mission Creator/C.H.A.O. Mission Creator.csproj`
- Delete: `Platforms/Android/`, `Platforms/iOS/`, `Platforms/MacCatalyst/`
- Delete: `Components/Pages/Counter.razor`, `Components/Pages/Home.razor`, `Components/Pages/Weather.razor`, `Components/Layout/NavMenu.razor`

- [ ] **Step 1: Replace csproj with Windows-only configuration**

```xml
<Project Sdk="Microsoft.NET.Sdk.Razor">
  <PropertyGroup>
    <TargetFrameworks>net10.0-windows10.0.19041.0</TargetFrameworks>
    <OutputType>Exe</OutputType>
    <RootNamespace>C.H.A.O._Mission_Creator</RootNamespace>
    <UseMaui>true</UseMaui>
    <SingleProject>true</SingleProject>
    <ImplicitUsings>enable</ImplicitUsings>
    <EnableDefaultCssItems>false</EnableDefaultCssItems>
    <Nullable>enable</Nullable>
    <MauiXamlInflator>SourceGen</MauiXamlInflator>
    <ApplicationTitle>C.H.A.O. Mission Creator</ApplicationTitle>
    <ApplicationId>com.soephond.chao.missioncreator</ApplicationId>
    <ApplicationDisplayVersion>1.0</ApplicationDisplayVersion>
    <ApplicationVersion>1</ApplicationVersion>
    <WindowsPackageType>None</WindowsPackageType>
    <SupportedOSPlatformVersion Condition="$([MSBuild]::GetTargetPlatformIdentifier('$(TargetFramework)')) == 'windows'">10.0.17763.0</SupportedOSPlatformVersion>
    <TargetPlatformMinVersion Condition="$([MSBuild]::GetTargetPlatformIdentifier('$(TargetFramework)')) == 'windows'">10.0.17763.0</TargetPlatformMinVersion>
  </PropertyGroup>
  <ItemGroup>
    <MauiIcon Include="Resources\AppIcon\appicon.svg" ForegroundFile="Resources\AppIcon\appiconfg.svg" Color="#512BD4"/>
    <MauiSplashScreen Include="Resources\Splash\splash.svg" Color="#512BD4" BaseSize="128,128"/>
    <MauiImage Include="Resources\Images\*"/>
    <MauiFont Include="Resources\Fonts\*"/>
    <MauiAsset Include="Resources\Raw\**" LogicalName="%(RecursiveDir)%(Filename)%(Extension)"/>
    <MauiAsset Include="chao_colors_reference.json" LogicalName="chao_colors_reference.json"/>
  </ItemGroup>
  <ItemGroup>
    <PackageReference Include="Microsoft.Maui.Controls" Version="$(MauiVersion)"/>
    <PackageReference Include="Microsoft.AspNetCore.Components.WebView.Maui" Version="$(MauiVersion)"/>
    <PackageReference Include="Microsoft.Extensions.Logging.Debug" Version="10.0.0"/>
  </ItemGroup>
</Project>
```

- [ ] **Step 2: Delete unused platform folders and pages**

```bash
# In the C.H.A.O. Mission Creator/ directory:
rm -rf Platforms/Android Platforms/iOS Platforms/MacCatalyst
rm Components/Pages/Counter.razor Components/Pages/Home.razor Components/Pages/Weather.razor
rm Components/Layout/NavMenu.razor
```

- [ ] **Step 3: Build to verify**

Run: `dotnet build "C.H.A.O. Mission Creator/C.H.A.O. Mission Creator.csproj" -f net10.0-windows10.0.19041.0`
Expected: Build succeeded (may warn about removed files — that's fine)

- [ ] **Step 4: Commit**

```bash
git add "C.H.A.O. Mission Creator/C.H.A.O. Mission Creator.csproj"
git add -u
git commit -m "chore: restrict to Windows-only target, remove unused platforms and pages"
```

---

## Task 2: Game Enums

**Files:**
- Create: `C.H.A.O. Mission Creator/Models/GameEnums.cs`

- [ ] **Step 1: Create the enums file**

```csharp
namespace C.H.A.O._Mission_Creator.Models;

public enum RequirementType
{
    TypeRequirement = 0,
    AppearanceRequirement = 1,
    StatRequirement = 2,
    AbilityRequirement = 3,
    ToyRequirement = 4,
    ClassesRequirement = 5,
    RaceRequirement = 6,
    KarateRequirement = 7,
    LifetimeRequirement = 8,
    BondRequirement = 10,
}

public enum RewardType
{
    RingReward = 0,
    FruitReward = 1,
    AnimalReward = 2,
    SeedReward = 3,
    EggReward = 4,
    HatReward = 5,
    ChaoReward = 6,
}

public enum ValueCheckType
{
    ChaoTypeCheck = 0,
    LevelCheck = 1,
    GradeCheck = 2,
    PointsCheck = 3,
    ColorCheck = 4,
    ToneCheck = 5,
    ShinyCheck = 6,
    TextureCheck = 7,
    AnimalArmsCheck = 8,
    AnimalEarsCheck = 9,
    AnimalForeheadCheck = 10,
    AnimalHornCheck = 11,
    AnimalLegsCheck = 12,
    AnimalTailCheck = 13,
    AnimalWingsCheck = 14,
    AnimalFaceCheck = 15,
    FlagCheck = 16,
    TotalCheck = 17,
    AmountWonCheck = 18,
    AgeCheck = 19,
    ReincarnationsCheck = 20,
    SwimFlyInfluenceCheck = 21,
    RunPowerInfluenceCheck = 22,
    AlignmentCheck = 23,
    MagnitudeCheck = 24,
    HappinessCheck = 25,
    CharacterLikeCheck = 26,
    CharacterFearCheck = 27,
    CharacterDistanceCheck = 28,
    CharacterMeetCheck = 29,
    KarateRankCheck = 30,
}

public enum RangeMode { Exact, Min, Max, Range }

public enum ChaoType
{
    Child = 2,
    Good = 3,
    Bad = 4,
    Neut_Normal = 5,
    Hero_Normal = 6,
    Dark_Normal = 7,
    Neut_Swim = 8,
    Hero_Swim = 9,
    Dark_Swim = 10,
    Neut_Fly = 11,
    Hero_Fly = 12,
    Dark_Fly = 13,
    Neut_Run = 14,
    Hero_Run = 15,
    Dark_Run = 16,
    Neut_Power = 17,
    Hero_Power = 18,
    Dark_Power = 19,
    Neut_Chaos = 20,
    Hero_Chaos = 21,
    Dark_Chaos = 22,
    Tails = 23,
    Knuckles = 24,
    Amy = 25,
}

public enum ChaoSkill { Swim = 0, Fly = 1, Run = 2, Power = 3, Stamina = 4, Luck = 5, Intelligence = 6 }

public enum StatGrade { E = 0, D = 1, C = 2, B = 3, A = 4, S = 5, X = 6 }

public enum ChaoTone { TwoTone = 0, MonoTone = 1 }

public enum ChaoShiny { None = 0, Bright = 1, Shiny = 2 }

public enum SA2BTexture
{
    None = 0, YellowJewel = 1, WhiteJewel = 2, PinkJewel = 3, BlueJewel = 4,
    GreenJewel = 5, PurpleJewel = 6, SkyBlueJewel = 7, RedJewel = 8, BlackJewel = 9,
    LimeGreenJewel = 10, OrangeJewel = 11, Pearl = 12, Metal1 = 13, Metal2 = 14,
    Glass = 15, Moon = 16,
}

public enum Al_Animal
{
    Penguin = 0, Seal = 1, Otter = 2, Rabbit = 3, Cheetah = 4, Warthog = 5,
    Bear = 6, Tiger = 7, Gorilla = 8, Peacock = 9, Parrot = 10, Condor = 11,
    Skunk = 12, Sheep = 13, Raccoon = 14, HalfFish = 15, SkeletonDog = 16,
    Bat = 17, Dragon = 18, Unicorn = 19, Phoenix = 20,
    YellowChaosDrive = 21, GreenChaosDrive = 22, RedChaosDrive = 23, PurpleChaosDrive = 24,
    CWE_Seal = 25, CWE_Penguin = 26, CWE_Otter = 27, CWE_Peacock = 28, CWE_Swallow = 29,
    CWE_Parrot = 30, CWE_Deer = 31, CWE_Rabbit = 32, CWE_Kangaroo = 33, CWE_Gorilla = 34,
    CWE_Lion = 35, CWE_Elephant = 36, CWE_Mole = 37, CWE_Koala = 38, CWE_Skunk = 39,
    CWE_Bee = 40, CWE_Ladybug = 41, CWE_Scorpion = 42,
    None = 255,
}

public enum SA2BFruit
{
    None = -1,
    ChaoGardenFruit = 0, HeroGardenFruit = 1, DarkGardenFruit = 2,
    StrongFruit = 3, TastyFruit = 4, HeroFruit = 5, DarkFruit = 6,
    RoundFruit = 7, TriangleFruit = 8, SquareFruit = 9, HeartFruit = 10,
    ChaoFruit = 11, SmartFruit = 12, OrangeFruit = 13, BlueFruit = 14,
    PinkFruit = 15, GreenFruit = 16, PurpleFruit = 17, YellowFruit = 18,
    RedFruit = 19, Mushroom = 20, MushroomAlt = 21, MintCandy = 22, Grapes = 23,
}

public enum ChaoSeed
{
    None = -1,
    StrongSeed = 0, TastySeed = 1, HeroSeed = 2, DarkSeed = 3,
    RoundSeed = 4, TriangleSeed = 5, SquareSeed = 6,
}

public enum SA2BHat
{
    None = 0, Pumpkin = 1, Skull = 2, Apple = 3, Bucket = 4, EmptyCan = 5,
    CardboardBox = 6, FlowerPot = 7, PaperBag = 8, Pan = 9, Stump = 10,
    Watermelon = 11, RedWoolBeanie = 12, BlueWoolBeanie = 13, BlackWoolBeanie = 14,
    Pacifier = 15, NormalEggShell = 16,
    Yellow_MonoTone_EggShell = 17, White_MonoTone_EggShell = 18, Brown_MonoTone_EggShell = 19,
    SkyBlue_MonoTone_EggShell = 20, Pink_MonoTone_EggShell = 21, Blue_MonoTone_EggShell = 22,
    Grey_MonoTone_EggShell = 23, Green_MonoTone_EggShell = 24, Red_MonoTone_EggShell = 25,
    LimeGreen_MonoTone_EggShell = 26, Purple_MonoTone_EggShell = 27, Orange_MonoTone_EggShell = 28,
    Black_MonoTone_EggShell = 29,
    Yellow_TwoTone_EggShell = 30, White_TwoTone_EggShell = 31, Brown_TwoTone_EggShell = 32,
    SkyBlue_TwoTone_EggShell = 33, Pink_TwoTone_EggShell = 34, Blue_TwoTone_EggShell = 35,
    Grey_TwoTone_EggShell = 36, Green_TwoTone_EggShell = 37, Red_TwoTone_EggShell = 38,
    LimeGreen_TwoTone_EggShell = 39, Purple_TwoTone_EggShell = 40, Orange_TwoTone_EggShell = 41,
    Black_TwoTone_EggShell = 42,
    NormalShinyEggShell = 43,
    YellowShiny_MonoTone_EggShell = 44, WhiteShiny_MonoTone_EggShell = 45,
    BrownShiny_MonoTone_EggShell = 46, SkyBlueShiny_MonoTone_EggShell = 47,
    PinkShiny_MonoTone_EggShell = 48, BlueShiny_MonoTone_EggShell = 49,
    GreyShiny_MonoTone_EggShell = 50, GreenShiny_MonoTone_EggShell = 51,
    RedShiny_MonoTone_EggShell = 52, LimeGreenShiny_MonoTone_EggShell = 53,
    PurpleShiny_MonoTone_EggShell = 54, OrangeShiny_MonoTone_EggShell = 55,
    BlackShiny_MonoTone_EggShell = 56,
    YellowShiny_TwoTone_EggShell = 57, WhiteShiny_TwoTone_EggShell = 58,
    BrownShiny_TwoTone_EggShell = 59, SkyBlueShiny_TwoTone_EggShell = 60,
    PinkShiny_TwoTone_EggShell = 61, BlueShiny_TwoTone_EggShell = 62,
    GreyShiny_TwoTone_EggShell = 63, GreenShiny_TwoTone_EggShell = 64,
    RedShiny_TwoTone_EggShell = 65, LimeGreenShiny_TwoTone_EggShell = 66,
    PurpleShiny_TwoTone_EggShell = 67, OrangeShiny_TwoTone_EggShell = 68,
    BlackShiny_TwoTone_EggShell = 69, GlitchyNormalEggShell = 70,
}

// eCHAO_EGGS is an enum class in C++ — magic_enum serializes as "eCHAO_EGGS::Normal"
// We use the same string prefix convention in the serializer.
public enum EChaoEggs
{
    Normal = 0,
    Yellow_MT = 1, White_MT = 2, Brown_MT = 3, Aqua_MT = 4, Pink_MT = 5,
    Blue_MT = 6, Gray_MT = 7, Green_MT = 8, Red_MT = 9, LightGreen_MT = 10,
    Purple_MT = 11, Orange_MT = 12, Black_MT = 13,
    Yellow_TT = 14, White_TT = 15, Brown_TT = 16, Aqua_TT = 17, Pink_TT = 18,
    Blue_TT = 19, Gray_TT = 20, Green_TT = 21, Red_TT = 22, LightGreen_TT = 23,
    Purple_TT = 24, Orange_TT = 25, Black_TT = 26,
    Normal_Shiny = 27,
    Yellow_SMT = 28, White_SMT = 29, Brown_SMT = 30, Aqua_SMT = 31, Pink_SMT = 32,
    Blue_SMT = 33, Gray_SMT = 34, Green_SMT = 35, Red_SMT = 36, LightGreen_SMT = 37,
    Purple_SMT = 38, Orange_SMT = 39, Black_SMT = 40,
    Yellow_STT = 41, White_STT = 42, Brown_STT = 43, Aqua_STT = 44, Pink_STT = 45,
    Blue_STT = 46, Gray_STT = 47, Green_STT = 48, Red_STT = 49, LightGreen_STT = 50,
    Purple_STT = 51, Orange_STT = 52, Black_STT = 53,
    Gold = 54, Silver = 55, Ruby = 56, Sapphire = 57, Emerald = 58,
    Amethyst = 59, Aquamarine = 60, Garnet = 61, Onyx = 62, Peridot = 63,
    Topaz = 64, Pearl = 65, Metal1 = 66, Metal2 = 67,
}

// eCHAO_TYPE is an enum class — magic_enum serializes as "eCHAO_TYPE::Child"
public enum EChaoType
{
    Empty = 0, Egg = 1, Child = 2, Good = 3, Bad = 4,
    Neut_Normal = 5, Hero_Normal = 6, Dark_Normal = 7,
    Neut_Swim = 8, Hero_Swim = 9, Dark_Swim = 10,
    Neut_Fly = 11, Hero_Fly = 12, Dark_Fly = 13,
    Neut_Run = 14, Hero_Run = 15, Dark_Run = 16,
    Neut_Power = 17, Hero_Power = 18, Dark_Power = 19,
    Neut_Chaos = 20, Hero_Chaos = 21, Dark_Chaos = 22,
    Tails = 23, Knuckles = 24, Amy = 25,
}

public enum CharacterBondOrder
{
    CharacterBond_Sonic = 0, CharacterBond_Shadow = 1, CharacterBond_Tails = 2,
    CharacterBond_Eggman = 3, CharacterBond_Knuckles = 4, CharacterBond_Rouge = 5,
}

public enum KarateRank
{
    NoRank = 0,
    TenthKyu = 1, NinthKyu = 2, EighthKyu = 3, SeventhKyu = 4, SixthKyu = 5,
    FifthKyu = 6, FourthKyu = 7, ThirdKyu = 8, SecondKyu = 9, FirstKyu = 10,
    FirstDegree = 11, SecondDegree = 12, ThirdDegree = 13, FourthDegree = 14,
    FifthDegree = 15, SixthDegree = 16, SeventhDegree = 17, EighthDegree = 18,
    NinthDegree = 19, GrandMaster = 20,
}

[Flags]
public enum MedalFlags : ushort
{
    None = 0,
    Challenge = 1 << 0, Beginner = 1 << 1, Hero = 1 << 2, Dark = 1 << 3,
    Aquamarine = 1 << 4, Topaz = 1 << 5, Peridot = 1 << 6, Garnet = 1 << 7,
    Onyx = 1 << 8, Diamond = 1 << 9, Pearl = 1 << 10, Amethyst = 1 << 11,
    Emerald = 1 << 12, Ruby = 1 << 13, Sapphire = 1 << 14,
}

[Flags]
public enum ChaoToyFlags : short
{
    None = 0,
    Rattle = 1 << 0, Car = 1 << 1, PictureBook = 1 << 2, SonicDoll = 1 << 4,
    Broomstick = 1 << 5, PogoStick = 1 << 7, Crayons = 1 << 8,
    BubbleWand = 1 << 9, Shovel = 1 << 10, WateringCan = 1 << 11,
}

[Flags]
public enum SA2BAnimalFlags : int
{
    None = 0,
    Penguin = 1 << 0, Seal = 1 << 1, Otter = 1 << 2, Rabbit = 1 << 3,
    Cheetah = 1 << 4, Warthog = 1 << 5, Bear = 1 << 6, Tiger = 1 << 7,
    Gorilla = 1 << 8, Peacock = 1 << 9, Parrot = 1 << 10, Condor = 1 << 11,
    Skunk = 1 << 12, Sheep = 1 << 13, Raccoon = 1 << 14, HalfFish = 1 << 15,
    SkeletonDog = 1 << 16, Bat = 1 << 17, Dragon = 1 << 18, Unicorn = 1 << 19,
    Phoenix = 1 << 20,
}

[Flags]
public enum ChaoClassroomFlags : int
{
    None = 0,
    DrawingLevel1 = 0x1, DrawingLevel2 = 0x2, DrawingLevel3 = 0x4,
    DrawingLevel4 = 0x8, DrawingLevel5 = 0x10,
    ShakeDance = 0x100, SpinDance = 0x200, StepDance = 0x400, GoGoDance = 0x800,
    Exercise = 0x1000,
    SongLevel1 = 0x10000, SongLevel2 = 0x20000, SongLevel3 = 0x40000,
    SongLevel4 = 0x80000, SongLevel5 = 0x100000,
    Bell = 0x1000000, Castanets = 0x2000000, Cymbals = 0x4000000,
    Drum = 0x8000000, Flute = 0x10000000, Maracas = 0x20000000,
    Trumpet = 0x40000000, Tambourine = unchecked((int)0x80000000),
}

// ChaoColor has ~200 entries. Only commonly-used named values shown here;
// the full list is derived from chao_colors_reference.json at runtime via ColorReferenceService.
// These string constants are used by the serializer for the 16 base colors.
public static class ChaoColorNames
{
    public const string Normal = "ChaoColor_Normal";
    public const string Yellow = "ChaoColor_Yellow";
    public const string White = "ChaoColor_White";
    public const string Brown = "ChaoColor_Brown";
    public const string SkyBlue = "ChaoColor_SkyBlue";
    public const string Pink = "ChaoColor_Pink";
    public const string Blue = "ChaoColor_Blue";
    public const string Grey = "ChaoColor_Grey";
    public const string Green = "ChaoColor_Green";
    public const string Red = "ChaoColor_Red";
    public const string LimeGreen = "ChaoColor_LimeGreen";
    public const string Purple = "ChaoColor_Purple";
    public const string Orange = "ChaoColor_Orange";
    public const string Black = "ChaoColor_Black";
    public const string PowderBlue = "ChaoColor_PowderBlue";
}
```

- [ ] **Step 2: Build to verify**

Run: `dotnet build "C.H.A.O. Mission Creator/C.H.A.O. Mission Creator.csproj" -f net10.0-windows10.0.19041.0`
Expected: Build succeeded

- [ ] **Step 3: Commit**

```bash
git add "C.H.A.O. Mission Creator/Models/GameEnums.cs"
git commit -m "feat: add game enum definitions"
```

---

## Task 3: Data Models

**Files:**
- Create: `C.H.A.O. Mission Creator/Models/MissionEditorModel.cs`
- Create: `C.H.A.O. Mission Creator/Models/RequirementModel.cs`
- Create: `C.H.A.O. Mission Creator/Models/RewardModel.cs`
- Create: `C.H.A.O. Mission Creator/Models/ColorReferenceData.cs`

- [ ] **Step 1: Create MissionEditorModel.cs**

```csharp
namespace C.H.A.O._Mission_Creator.Models;

public class MissionEditorModel
{
    public string Name { get; set; } = "";
    public List<string> DescriptionLines { get; set; } = new();
    public List<RequirementModel> Requirements { get; set; } = new();
    public List<RewardModel> Rewards { get; set; } = new();
    public List<RequirementModel> BonusRequirements { get; set; } = new();
    public List<RewardModel> BonusRewards { get; set; } = new();
}
```

- [ ] **Step 2: Create RequirementModel.cs**

```csharp
namespace C.H.A.O._Mission_Creator.Models;

public class RequirementModel
{
    public RequirementType Type { get; set; } = RequirementType.TypeRequirement;
    public string Description { get; set; } = "";
    // Outer list = OR branches; inner list = AND conditions within a branch
    public List<List<CheckModel>> Checks { get; set; } = new() { new() { new() } };
}

public class CheckModel
{
    // Matches ValueCheckType enum name, e.g. "LevelCheck"
    public string CheckType { get; set; } = nameof(ValueCheckType.ChaoTypeCheck);
    public RangeMode RangeMode { get; set; } = RangeMode.Exact;
    public bool Inverted { get; set; }

    // Enum name string or numeric string for Value/MinValue/MaxValue
    public string? Value { get; set; }
    public string? MinValue { get; set; }
    public string? MaxValue { get; set; }

    // For stat and happiness checks
    public string? Skill { get; set; }

    // For bond checks
    public string? Character { get; set; }

    // For ColorCheck in group mode (non-advanced): group name e.g. "Red"
    // When set, serializer expands to one OR branch per color in group.
    // When null, Value holds the exact enum name (advanced mode).
    public string? ColorGroup { get; set; }
}
```

- [ ] **Step 3: Create RewardModel.cs**

```csharp
namespace C.H.A.O._Mission_Creator.Models;

public class RewardModel
{
    public RewardType Type { get; set; } = RewardType.RingReward;
    public int Amount { get; set; } = 1;
    public string Description { get; set; } = "";

    // Enum name string for Fruit/Animal/Seed/Egg/Hat rewards (e.g. "SA2BFruit_HeroFruit")
    public string? EnumValue { get; set; }

    // Only used when Type == ChaoReward
    public ChaoRewardValue? ChaoValue { get; set; }
}

public class ChaoRewardValue
{
    // Stored as magic_enum strings: "eCHAO_TYPE::Child", "ChaoColor_Normal", etc.
    public string ChaoType { get; set; } = "eCHAO_TYPE::Child";
    public string Color { get; set; } = "ChaoColor_Normal";
    public string Texture { get; set; } = "SA2BTexture_None";
    public string Tone { get; set; } = "ChaoTone_MonoTone";
    public string Shiny { get; set; } = "ChaoShiny_None";
    public string Name { get; set; } = "";
}
```

- [ ] **Step 4: Create ColorReferenceData.cs**

```csharp
namespace C.H.A.O._Mission_Creator.Models;

public record ColorEntry(string EnumName, int Value, string Hex, string Group);

public record ColorGroup(string Name, string RepresentativeHex, IReadOnlyList<ColorEntry> Colors);
```

- [ ] **Step 5: Build to verify**

Run: `dotnet build "C.H.A.O. Mission Creator/C.H.A.O. Mission Creator.csproj" -f net10.0-windows10.0.19041.0`
Expected: Build succeeded

- [ ] **Step 6: Commit**

```bash
git add "C.H.A.O. Mission Creator/Models/"
git commit -m "feat: add mission editor data models"
```

---

## Task 4: ColorReferenceService

**Files:**
- Create: `C.H.A.O. Mission Creator/Services/ColorReferenceService.cs`

The `chao_colors_reference.json` file is already present at the project root and registered as a MauiAsset. Its structure:
```json
{
  "colors": [
    { "enum_name": "ChaoColor_Normal", "value": 0, "hex": "#A0C8C8", "group": "Normal" },
    ...
  ]
}
```

- [ ] **Step 1: Create ColorReferenceService.cs**

```csharp
using System.Text.Json;
using C.H.A.O._Mission_Creator.Models;

namespace C.H.A.O._Mission_Creator.Services;

public class ColorReferenceService
{
    private List<ColorEntry>? _entries;
    private List<ColorGroup>? _groups;

    public async Task EnsureLoadedAsync()
    {
        if (_entries is not null) return;

        await using var stream = await FileSystem.OpenAppPackageFileAsync("chao_colors_reference.json");
        using var doc = await JsonDocument.ParseAsync(stream);
        var arr = doc.RootElement.GetProperty("colors");

        _entries = new List<ColorEntry>();
        foreach (var el in arr.EnumerateArray())
        {
            _entries.Add(new ColorEntry(
                el.GetProperty("enum_name").GetString()!,
                el.GetProperty("value").GetInt32(),
                el.GetProperty("hex").GetString()!,
                el.GetProperty("group").GetString()!
            ));
        }

        var groupOrder = new[] {
            "Normal","Red","Orange","Yellow","Lime Green","Green",
            "Powder Blue","Sky Blue","Blue","Dark Blue","Purple",
            "Pink","White","Grey","Black","Brown"
        };
        var repHex = new Dictionary<string, string> {
            ["Normal"]="#A0C8C8",["Red"]="#FF0000",["Orange"]="#FF8600",
            ["Yellow"]="#FFFF00",["Lime Green"]="#B1FF00",["Green"]="#009E00",
            ["Powder Blue"]="#ACF0FF",["Sky Blue"]="#00D6FA",["Blue"]="#4550FF",
            ["Dark Blue"]="#0700F5",["Purple"]="#BA00FF",["Pink"]="#FF81E8",
            ["White"]="#FFFFFF",["Grey"]="#959595",["Black"]="#404040",["Brown"]="#9E6E00"
        };

        var byGroup = _entries.GroupBy(e => e.Group)
            .ToDictionary(g => g.Key, g => g.ToList());

        _groups = groupOrder
            .Where(name => byGroup.ContainsKey(name))
            .Select(name => new ColorGroup(name, repHex[name], byGroup[name]))
            .ToList();
    }

    public IReadOnlyList<ColorGroup> Groups => _groups ?? [];
    public IReadOnlyList<ColorEntry> AllColors => _entries ?? [];

    public string? GetGroupForEnum(string enumName) =>
        _entries?.FirstOrDefault(e => e.EnumName == enumName)?.Group;

    public IEnumerable<ColorEntry> GetColorsForGroup(string groupName) =>
        _entries?.Where(e => e.Group == groupName) ?? [];
}
```

- [ ] **Step 2: Build to verify**

Run: `dotnet build "C.H.A.O. Mission Creator/C.H.A.O. Mission Creator.csproj" -f net10.0-windows10.0.19041.0`
Expected: Build succeeded

- [ ] **Step 3: Commit**

```bash
git add "C.H.A.O. Mission Creator/Services/ColorReferenceService.cs"
git commit -m "feat: add ColorReferenceService"
```

---

## Task 5: AppSettingsService

**Files:**
- Create: `C.H.A.O. Mission Creator/Services/AppSettingsService.cs`

- [ ] **Step 1: Create AppSettingsService.cs**

```csharp
using System.Text.Json;

namespace C.H.A.O._Mission_Creator.Services;

public class AppSettingsService
{
    private static readonly string SettingsPath = Path.Combine(
        Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData),
        "ChaoMissionCreator", "settings.json");

    public string? Sa2Path { get; set; }
    public bool EnableAllChecks { get; set; }
    public bool EnableAdvancedColorCheck { get; set; }

    public async Task LoadAsync()
    {
        if (!File.Exists(SettingsPath)) return;
        try
        {
            await using var stream = File.OpenRead(SettingsPath);
            var doc = await JsonDocument.ParseAsync(stream);
            var root = doc.RootElement;
            if (root.TryGetProperty("sa2Path", out var p)) Sa2Path = p.GetString();
            if (root.TryGetProperty("enableAllChecks", out var e)) EnableAllChecks = e.GetBoolean();
            if (root.TryGetProperty("enableAdvancedColorCheck", out var a)) EnableAdvancedColorCheck = a.GetBoolean();
        }
        catch { /* corrupt settings — ignore */ }
    }

    public async Task SaveAsync()
    {
        Directory.CreateDirectory(Path.GetDirectoryName(SettingsPath)!);
        await using var stream = File.Create(SettingsPath);
        await JsonSerializer.SerializeAsync(stream, new
        {
            sa2Path = Sa2Path,
            enableAllChecks = EnableAllChecks,
            enableAdvancedColorCheck = EnableAdvancedColorCheck,
        }, new JsonSerializerOptions { WriteIndented = true });
    }
}
```

- [ ] **Step 2: Build to verify**

Run: `dotnet build "C.H.A.O. Mission Creator/C.H.A.O. Mission Creator.csproj" -f net10.0-windows10.0.19041.0`
Expected: Build succeeded

- [ ] **Step 3: Commit**

```bash
git add "C.H.A.O. Mission Creator/Services/AppSettingsService.cs"
git commit -m "feat: add AppSettingsService"
```

---

## Task 6: MissionSerializer — Deserialize

**Files:**
- Create: `C.H.A.O. Mission Creator/Services/MissionSerializer.cs`

JSON format facts (from `JsonMissionReader.cpp`):
- Top-level keys: `Name`, `Description`, `Requirements`, `Rewards`, `"Bonus Requirements"`, `"Bonus Rewards"` (spaces in bonus keys)
- Each requirement: `{ "Type": "StatRequirement", "Description": "...", "Checks": [[...], [...]] }`
- `Checks` is array-of-arrays: outer = OR branches, inner = AND conditions
- Each check: `{ "Type": "LevelCheck", "Skill": "ChaoSkill_Run", "MinValue": 10 }`
- Range determined by which of `Value`/`MinValue`/`MaxValue` are present
- `eCHAO_TYPE` and `eCHAO_EGGS` are `enum class` — serialized as `"eCHAO_TYPE::Child"` by magic_enum
- Color group collapse: if all OR branches of a requirement are each a single `ColorCheck` and all colors share the same group → use `ColorGroup` mode on the `CheckModel`

- [ ] **Step 1: Create MissionSerializer.cs with Deserialize**

```csharp
using System.Text;
using System.Text.Json;
using C.H.A.O._Mission_Creator.Models;

namespace C.H.A.O._Mission_Creator.Services;

public class MissionSerializer
{
    private readonly ColorReferenceService _colors;

    public MissionSerializer(ColorReferenceService colors) => _colors = colors;

    // ── Deserialize ──────────────────────────────────────────────────────────

    public MissionEditorModel Deserialize(string json)
    {
        using var doc = JsonDocument.Parse(json);
        var root = doc.RootElement;
        var model = new MissionEditorModel();

        model.Name = root.GetProperty("Name").GetString() ?? "";

        foreach (var line in root.GetProperty("Description").EnumerateArray())
            model.DescriptionLines.Add(line.GetString() ?? "");

        model.Requirements = ReadRequirements(root.GetProperty("Requirements"));
        model.Rewards = ReadRewards(root.GetProperty("Rewards"));

        if (root.TryGetProperty("Bonus Requirements", out var bonusReqs))
            model.BonusRequirements = ReadRequirements(bonusReqs);

        if (root.TryGetProperty("Bonus Rewards", out var bonusRews))
            model.BonusRewards = ReadRewards(bonusRews);

        return model;
    }

    private List<RequirementModel> ReadRequirements(JsonElement el)
    {
        var list = new List<RequirementModel>();
        foreach (var req in el.EnumerateArray())
            list.Add(ReadRequirement(req));
        return list;
    }

    private RequirementModel ReadRequirement(JsonElement el)
    {
        var model = new RequirementModel
        {
            Type = Enum.Parse<RequirementType>(el.GetProperty("Type").GetString()!),
            Description = el.GetProperty("Description").GetString() ?? "",
            Checks = new(),
        };

        foreach (var branch in el.GetProperty("Checks").EnumerateArray())
        {
            var andList = new List<CheckModel>();
            foreach (var check in branch.EnumerateArray())
                andList.Add(ReadCheck(check, model.Type));
            model.Checks.Add(andList);
        }

        // Collapse color branches to group mode if possible
        TryCollapseColorGroup(model);

        return model;
    }

    private CheckModel ReadCheck(JsonElement el, RequirementType reqType)
    {
        var checkType = el.GetProperty("Type").GetString()!;
        var cm = new CheckModel { CheckType = checkType };

        // Value / MinValue / MaxValue
        bool hasValue = el.TryGetProperty("Value", out var valEl);
        bool hasMin   = el.TryGetProperty("MinValue", out var minEl);
        bool hasMax   = el.TryGetProperty("MaxValue", out var maxEl);

        cm.Value    = hasValue ? ReadValueString(valEl) : null;
        cm.MinValue = hasMin   ? ReadValueString(minEl) : null;
        cm.MaxValue = hasMax   ? ReadValueString(maxEl) : null;

        cm.RangeMode = (hasValue, hasMin, hasMax) switch
        {
            (true, _, _)      => RangeMode.Exact,
            (false, true, true) => RangeMode.Range,
            (false, true, false) => RangeMode.Min,
            (false, false, true) => RangeMode.Max,
            _ => RangeMode.Exact,
        };

        if (el.TryGetProperty("Inverted", out var inv))
            cm.Inverted = inv.GetBoolean();

        if (el.TryGetProperty("Skill", out var skill))
            cm.Skill = skill.GetString();

        if (el.TryGetProperty("Character", out var ch))
            cm.Character = ch.GetString();

        return cm;
    }

    private static string ReadValueString(JsonElement el) =>
        el.ValueKind == JsonValueKind.String ? el.GetString()! : el.GetRawText();

    private void TryCollapseColorGroup(RequirementModel req)
    {
        // Only collapse if every OR branch is a single ColorCheck
        if (req.Checks.Count == 0) return;
        if (!req.Checks.All(branch => branch.Count == 1 &&
            branch[0].CheckType == nameof(ValueCheckType.ColorCheck) &&
            branch[0].Value is not null &&
            branch[0].RangeMode == RangeMode.Exact &&
            !branch[0].Inverted))
            return;

        var enumNames = req.Checks.Select(b => b[0].Value!).ToList();
        var groups = enumNames.Select(n => _colors.GetGroupForEnum(n)).ToList();

        if (groups.Any(g => g is null)) return;
        if (groups.Distinct().Count() != 1) return;

        // All same group — collapse to single branch with ColorGroup set
        var groupName = groups[0]!;
        req.Checks = new List<List<CheckModel>>
        {
            new() { new CheckModel { CheckType = nameof(ValueCheckType.ColorCheck), ColorGroup = groupName } }
        };
    }

    private List<RewardModel> ReadRewards(JsonElement el)
    {
        var list = new List<RewardModel>();
        foreach (var rew in el.EnumerateArray())
            list.Add(ReadReward(rew));
        return list;
    }

    private static RewardModel ReadReward(JsonElement el)
    {
        var type = Enum.Parse<RewardType>(el.GetProperty("Type").GetString()!);
        var model = new RewardModel
        {
            Type = type,
            Description = el.GetProperty("Description").GetString() ?? "",
        };

        if (el.TryGetProperty("Amount", out var amt))
            model.Amount = amt.GetInt32();

        if (type == RewardType.ChaoReward)
        {
            var v = el.GetProperty("Value");
            model.ChaoValue = new ChaoRewardValue
            {
                ChaoType = v.TryGetProperty("ChaoType", out var ct)  ? ReadValueString(ct) : "eCHAO_TYPE::Child",
                Color    = v.TryGetProperty("Color",    out var co)  ? ReadValueString(co) : "ChaoColor_Normal",
                Texture  = v.TryGetProperty("Texture",  out var tx)  ? ReadValueString(tx) : "SA2BTexture_None",
                Tone     = v.TryGetProperty("Tone",     out var tn)  ? ReadValueString(tn) : "ChaoTone_MonoTone",
                Shiny    = v.TryGetProperty("Shiny",    out var sh)  ? ReadValueString(sh) : "ChaoShiny_None",
                Name     = v.TryGetProperty("Name",     out var nm)  ? nm.GetString() ?? "" : "",
            };
        }
        else if (type != RewardType.RingReward && el.TryGetProperty("Value", out var enumVal))
        {
            model.EnumValue = ReadValueString(enumVal);
        }

        return model;
    }
}
```

- [ ] **Step 2: Build to verify**

Run: `dotnet build "C.H.A.O. Mission Creator/C.H.A.O. Mission Creator.csproj" -f net10.0-windows10.0.19041.0`
Expected: Build succeeded

- [ ] **Step 3: Commit**

```bash
git add "C.H.A.O. Mission Creator/Services/MissionSerializer.cs"
git commit -m "feat: add MissionSerializer deserialization"
```

---

## Task 7: MissionSerializer — Serialize

**Files:**
- Modify: `C.H.A.O. Mission Creator/Services/MissionSerializer.cs`

Key rules:
- Always output enum string names, never integers
- `ColorGroup` mode → expand to one OR branch per color in the group, each branch = single `ColorCheck` with `"Value": "ChaoColor_xxx"`
- `eCHAO_TYPE` and `eCHAO_EGGS` values already stored as `"eCHAO_TYPE::Child"` etc. — write as-is
- Output is indented JSON

- [ ] **Step 1: Add Serialize method to MissionSerializer.cs**

Add the following methods inside the `MissionSerializer` class (after the `Deserialize` region):

```csharp
    // ── Serialize ────────────────────────────────────────────────────────────

    public string Serialize(MissionEditorModel model)
    {
        var opts = new JsonWriterOptions { Indented = true };
        using var ms = new MemoryStream();
        using var w = new Utf8JsonWriter(ms, opts);

        w.WriteStartObject();
        w.WriteString("Name", model.Name);

        w.WriteStartArray("Description");
        foreach (var line in model.DescriptionLines) w.WriteStringValue(line);
        w.WriteEndArray();

        w.WriteStartArray("Requirements");
        foreach (var req in model.Requirements) WriteRequirement(w, req);
        w.WriteEndArray();

        w.WriteStartArray("Rewards");
        foreach (var rew in model.Rewards) WriteReward(w, rew);
        w.WriteEndArray();

        w.WriteStartArray("Bonus Requirements");
        foreach (var req in model.BonusRequirements) WriteRequirement(w, req);
        w.WriteEndArray();

        w.WriteStartArray("Bonus Rewards");
        foreach (var rew in model.BonusRewards) WriteReward(w, rew);
        w.WriteEndArray();

        w.WriteEndObject();
        w.Flush();
        return Encoding.UTF8.GetString(ms.ToArray());
    }

    private void WriteRequirement(Utf8JsonWriter w, RequirementModel req)
    {
        w.WriteStartObject();
        w.WriteString("Type", req.Type.ToString());
        w.WriteString("Description", req.Description);
        w.WriteStartArray("Checks");

        foreach (var branch in req.Checks)
        {
            // Expand color group into one OR branch per color
            if (branch.Count == 1 && branch[0].ColorGroup is { } grp)
            {
                foreach (var entry in _colors.GetColorsForGroup(grp))
                {
                    w.WriteStartArray();
                    w.WriteStartObject();
                    w.WriteString("Type", nameof(ValueCheckType.ColorCheck));
                    w.WriteString("Value", entry.EnumName);
                    w.WriteEndObject();
                    w.WriteEndArray();
                }
            }
            else
            {
                w.WriteStartArray();
                foreach (var check in branch) WriteCheck(w, check);
                w.WriteEndArray();
            }
        }

        w.WriteEndArray();
        w.WriteEndObject();
    }

    private static void WriteCheck(Utf8JsonWriter w, CheckModel c)
    {
        w.WriteStartObject();
        w.WriteString("Type", c.CheckType);

        if (c.Skill is not null) w.WriteString("Skill", c.Skill);
        if (c.Character is not null) w.WriteString("Character", c.Character);

        switch (c.RangeMode)
        {
            case RangeMode.Exact:
                WriteValue(w, "Value", c.Value);
                break;
            case RangeMode.Min:
                WriteValue(w, "MinValue", c.MinValue);
                break;
            case RangeMode.Max:
                WriteValue(w, "MaxValue", c.MaxValue);
                break;
            case RangeMode.Range:
                WriteValue(w, "MinValue", c.MinValue);
                WriteValue(w, "MaxValue", c.MaxValue);
                break;
        }

        if (c.Inverted) w.WriteBoolean("Inverted", true);
        w.WriteEndObject();
    }

    private static void WriteValue(Utf8JsonWriter w, string key, string? val)
    {
        if (val is null) return;
        // If the stored value is a plain integer string, write as number; otherwise as string
        if (int.TryParse(val, out var n))
            w.WriteNumber(key, n);
        else
            w.WriteString(key, val);
    }

    private static void WriteReward(Utf8JsonWriter w, RewardModel r)
    {
        w.WriteStartObject();
        w.WriteString("Type", r.Type.ToString());
        w.WriteNumber("Amount", r.Amount);

        if (r.Type == RewardType.ChaoReward && r.ChaoValue is { } cv)
        {
            w.WriteStartObject("Value");
            w.WriteString("ChaoType", cv.ChaoType);
            w.WriteString("Color", cv.Color);
            w.WriteString("Texture", cv.Texture);
            w.WriteString("Tone", cv.Tone);
            w.WriteString("Shiny", cv.Shiny);
            w.WriteString("Name", cv.Name);
            w.WriteEndObject();
        }
        else if (r.EnumValue is not null)
        {
            if (int.TryParse(r.EnumValue, out var n))
                w.WriteNumber("Value", n);
            else
                w.WriteString("Value", r.EnumValue);
        }

        w.WriteString("Description", r.Description);
        w.WriteEndObject();
    }
```

- [ ] **Step 2: Build to verify**

Run: `dotnet build "C.H.A.O. Mission Creator/C.H.A.O. Mission Creator.csproj" -f net10.0-windows10.0.19041.0`
Expected: Build succeeded

- [ ] **Step 3: Quick round-trip smoke test**

Paste this into a temporary test method or run in a scratch file to verify:
```csharp
var json = File.ReadAllText(@"C:\Git\Chao-Mission\C.H.A.O\Missions\InProgress\ChaoFarmer.json");
var colorSvc = new ColorReferenceService();
await colorSvc.EnsureLoadedAsync();
var serializer = new MissionSerializer(colorSvc);
var model = serializer.Deserialize(json);
var roundTripped = serializer.Serialize(model);
// Verify roundTripped is valid JSON with same structure
```

- [ ] **Step 4: Commit**

```bash
git add "C.H.A.O. Mission Creator/Services/MissionSerializer.cs"
git commit -m "feat: add MissionSerializer serialization with color group expansion"
```

---

## Task 8: MissionFileService

**Files:**
- Create: `C.H.A.O. Mission Creator/Services/MissionFileService.cs`

Uses `Microsoft.Win32.OpenFileDialog` / `SaveFileDialog` which are available on Windows without extra packages (part of `PresentationFramework` via WinForms/WPF interop — but in a MAUI Windows app, use `Windows.Storage.Pickers` or P/Invoke). 

**Important:** MAUI Windows apps use `WinRT` file pickers. Use `Windows.Storage.Pickers.FileSavePicker` via the `WinRT.Interop` helpers. The HWND is obtained from `Application.Current.Windows[0].Handler.PlatformView`.

- [ ] **Step 1: Create MissionFileService.cs**

```csharp
using C.H.A.O._Mission_Creator.Models;
using Windows.Storage;
using Windows.Storage.Pickers;
using WinRT.Interop;

namespace C.H.A.O._Mission_Creator.Services;

public class MissionFileService
{
    private readonly MissionSerializer _serializer;

    public MissionFileService(MissionSerializer serializer) => _serializer = serializer;

    public MissionEditorModel Load(string filePath)
    {
        var json = File.ReadAllText(filePath);
        return _serializer.Deserialize(json);
    }

    public async Task<string?> SaveAsAsync(MissionEditorModel model)
    {
        var picker = new FileSavePicker
        {
            SuggestedStartLocation = PickerLocationId.DocumentsLibrary,
            SuggestedFileName = string.IsNullOrWhiteSpace(model.Name) ? "mission" : model.Name,
        };
        picker.FileTypeChoices.Add("Mission JSON", new List<string> { ".json" });

        // Initialize picker with the app window handle (required on Windows)
        var hwnd = GetAppHwnd();
        InitializeWithWindow.Initialize(picker, hwnd);

        var file = await picker.PickSaveFileAsync();
        if (file is null) return null;

        var json = _serializer.Serialize(model);
        await FileIO.WriteTextAsync(file, json);
        return file.Path;
    }

    public async Task<string> SaveToBacklogAsync(MissionEditorModel model, string sa2Path)
    {
        var dir = Path.Combine(sa2Path, "mods", "C.H.A.O", "Missions", "Backlog");
        Directory.CreateDirectory(dir);

        var name = string.IsNullOrWhiteSpace(model.Name) ? "mission" : model.Name;
        var path = Path.Combine(dir, $"{name}.json");
        var json = _serializer.Serialize(model);
        await File.WriteAllTextAsync(path, json);
        return path;
    }

    private static IntPtr GetAppHwnd()
    {
        var window = Application.Current?.Windows[0];
        if (window?.Handler?.PlatformView is Microsoft.UI.Xaml.Window nativeWindow)
            return WindowNative.GetWindowHandle(nativeWindow);
        return IntPtr.Zero;
    }
}
```

- [ ] **Step 2: Build to verify**

Run: `dotnet build "C.H.A.O. Mission Creator/C.H.A.O. Mission Creator.csproj" -f net10.0-windows10.0.19041.0`
Expected: Build succeeded (WinRT types are available in the Windows target)

- [ ] **Step 3: Commit**

```bash
git add "C.H.A.O. Mission Creator/Services/MissionFileService.cs"
git commit -m "feat: add MissionFileService with WinRT file pickers"
```

---

## Task 9: Register Services + Update _Imports

**Files:**
- Modify: `C.H.A.O. Mission Creator/MauiProgram.cs`
- Modify: `C.H.A.O. Mission Creator/Components/_Imports.razor`

- [ ] **Step 1: Update MauiProgram.cs**

```csharp
using C.H.A.O._Mission_Creator.Services;
using Microsoft.Extensions.Logging;

namespace C.H.A.O._Mission_Creator;

public static class MauiProgram
{
    public static MauiApp CreateMauiApp()
    {
        var builder = MauiApp.CreateBuilder();
        builder
            .UseMauiApp<App>()
            .ConfigureFonts(fonts => { fonts.AddFont("OpenSans-Regular.ttf", "OpenSansRegular"); });

        builder.Services.AddMauiBlazorWebView();
        builder.Services.AddSingleton<ColorReferenceService>();
        builder.Services.AddSingleton<AppSettingsService>();
        builder.Services.AddSingleton<MissionSerializer>();
        builder.Services.AddSingleton<MissionFileService>();

#if DEBUG
        builder.Services.AddBlazorWebViewDeveloperTools();
        builder.Logging.AddDebug();
#endif

        return builder.Build();
    }
}
```

- [ ] **Step 2: Update _Imports.razor**

```razor
@using System.Net.Http
@using System.Net.Http.Json
@using Microsoft.AspNetCore.Components.Forms
@using Microsoft.AspNetCore.Components.Routing
@using Microsoft.AspNetCore.Components.Web
@using Microsoft.AspNetCore.Components.Web.Virtualization
@using Microsoft.JSInterop
@using C.H.A.O._Mission_Creator
@using C.H.A.O._Mission_Creator.Components
@using C.H.A.O._Mission_Creator.Components.Layout
@using C.H.A.O._Mission_Creator.Models
@using C.H.A.O._Mission_Creator.Services
```

- [ ] **Step 3: Build to verify**

Run: `dotnet build "C.H.A.O. Mission Creator/C.H.A.O. Mission Creator.csproj" -f net10.0-windows10.0.19041.0`
Expected: Build succeeded

- [ ] **Step 4: Commit**

```bash
git add "C.H.A.O. Mission Creator/MauiProgram.cs"
git add "C.H.A.O. Mission Creator/Components/_Imports.razor"
git commit -m "feat: register services, update imports"
```

---

## Task 10: CSS Layout

**Files:**
- Replace: `C.H.A.O. Mission Creator/wwwroot/css/app.css`
- Create: `C.H.A.O. Mission Creator/wwwroot/js/dropzone.js`

- [ ] **Step 1: Replace app.css**

```css
/* Reset & base */
*, *::before, *::after { box-sizing: border-box; margin: 0; padding: 0; }

html, body, #app {
    height: 100%;
    font-family: 'Segoe UI', system-ui, sans-serif;
    font-size: 14px;
    background: #1a1a2e;
    color: #e0e0f0;
}

/* ── App shell ─────────────────────────────────────────────────────────── */
.app-shell {
    display: grid;
    grid-template-rows: 44px 1fr 48px;
    height: 100vh;
    overflow: hidden;
}

/* ── Header ────────────────────────────────────────────────────────────── */
.app-header {
    display: flex;
    align-items: center;
    justify-content: space-between;
    padding: 0 16px;
    background: #0f0f23;
    border-bottom: 1px solid #2a2a4a;
    font-weight: 600;
    letter-spacing: 0.05em;
    color: #a0c8c8;
}
.app-header button.icon-btn {
    background: none;
    border: none;
    color: #a0c8c8;
    font-size: 18px;
    cursor: pointer;
    padding: 4px 8px;
    border-radius: 4px;
}
.app-header button.icon-btn:hover { background: #2a2a4a; }

/* ── Main area ─────────────────────────────────────────────────────────── */
.main-area {
    display: grid;
    grid-template-columns: 380px 1fr;
    overflow: hidden;
}

/* ── Left panel ────────────────────────────────────────────────────────── */
.left-panel {
    display: flex;
    flex-direction: column;
    gap: 12px;
    padding: 16px;
    border-right: 1px solid #2a2a4a;
    background: #16162a;
    overflow-y: auto;
}
.left-panel label { display: block; font-size: 11px; color: #7878a0; margin-bottom: 4px; text-transform: uppercase; letter-spacing: 0.06em; }
.left-panel input[type="text"], .left-panel textarea {
    width: 100%;
    background: #0f0f23;
    border: 1px solid #2a2a4a;
    border-radius: 4px;
    color: #e0e0f0;
    padding: 6px 8px;
    font-size: 14px;
}
.left-panel input[type="text"]:focus, .left-panel textarea:focus {
    outline: none;
    border-color: #a0c8c8;
}

/* ── Right panel ───────────────────────────────────────────────────────── */
.right-panel {
    display: flex;
    flex-direction: column;
    overflow: hidden;
}

/* ── Tabs ──────────────────────────────────────────────────────────────── */
.tab-bar {
    display: flex;
    gap: 2px;
    padding: 8px 12px 0;
    background: #16162a;
    border-bottom: 1px solid #2a2a4a;
    flex-shrink: 0;
}
.tab-bar button {
    background: none;
    border: none;
    border-bottom: 2px solid transparent;
    color: #7878a0;
    cursor: pointer;
    font-size: 13px;
    padding: 6px 16px;
    border-radius: 4px 4px 0 0;
    transition: color 0.15s, border-color 0.15s;
}
.tab-bar button.active { color: #a0c8c8; border-bottom-color: #a0c8c8; }
.tab-bar button:hover:not(.active) { color: #c0c0e0; background: #1e1e3a; }

/* ── Tab content ───────────────────────────────────────────────────────── */
.tab-content {
    display: grid;
    grid-template-columns: 1fr 1fr;
    flex: 1;
    overflow: hidden;
}
.panel-col {
    display: flex;
    flex-direction: column;
    overflow: hidden;
    border-right: 1px solid #2a2a4a;
}
.panel-col:last-child { border-right: none; }
.panel-header {
    display: flex;
    align-items: center;
    justify-content: space-between;
    padding: 8px 12px;
    background: #1a1a2e;
    border-bottom: 1px solid #2a2a4a;
    flex-shrink: 0;
    font-size: 11px;
    text-transform: uppercase;
    letter-spacing: 0.06em;
    color: #7878a0;
}
.panel-scroll { flex: 1; overflow-y: auto; padding: 8px; display: flex; flex-direction: column; gap: 8px; }

/* ── Cards ─────────────────────────────────────────────────────────────── */
.card {
    background: #1e1e38;
    border: 1px solid #2a2a4a;
    border-radius: 6px;
    padding: 10px 12px;
}
.card-header {
    display: flex;
    align-items: center;
    gap: 8px;
    margin-bottom: 8px;
}
.card select, .card input[type="text"], .card input[type="number"] {
    background: #0f0f23;
    border: 1px solid #2a2a4a;
    border-radius: 4px;
    color: #e0e0f0;
    padding: 4px 6px;
    font-size: 13px;
}
.card select:focus, .card input:focus { outline: none; border-color: #a0c8c8; }
.card select { cursor: pointer; }

.or-separator { text-align: center; color: #5050a0; font-size: 11px; font-weight: 600; margin: 4px 0; }
.or-branch { background: #161628; border: 1px dashed #2a2a4a; border-radius: 4px; padding: 8px; margin-bottom: 4px; }
.check-row { display: flex; flex-wrap: wrap; gap: 6px; align-items: center; margin-bottom: 4px; }

/* ── Buttons ───────────────────────────────────────────────────────────── */
.btn { padding: 5px 12px; border-radius: 4px; border: none; cursor: pointer; font-size: 13px; }
.btn-primary { background: #3a3a7a; color: #e0e0f0; }
.btn-primary:hover { background: #4a4a9a; }
.btn-secondary { background: #2a2a4a; color: #a0a0c0; }
.btn-secondary:hover { background: #3a3a5a; }
.btn-danger { background: none; color: #c05050; padding: 2px 6px; font-size: 12px; }
.btn-danger:hover { background: #3a1a1a; }
.btn-add { background: none; border: 1px dashed #3a3a6a; color: #7878a0; width: 100%; padding: 6px; border-radius: 4px; cursor: pointer; font-size: 12px; }
.btn-add:hover { background: #1e1e3a; color: #a0c8c8; border-color: #a0c8c8; }

/* ── Save bar ──────────────────────────────────────────────────────────── */
.save-bar {
    display: flex;
    align-items: center;
    gap: 10px;
    padding: 0 16px;
    background: #0f0f23;
    border-top: 1px solid #2a2a4a;
}
.save-bar .status { flex: 1; font-size: 12px; color: #7878a0; overflow: hidden; text-overflow: ellipsis; white-space: nowrap; }
.status-ok { color: #50a050 !important; }
.status-err { color: #c05050 !important; }

/* ── Drop zone ─────────────────────────────────────────────────────────── */
.drop-zone {
    border: 2px dashed #3a3a6a;
    border-radius: 6px;
    padding: 24px 16px;
    text-align: center;
    color: #5050a0;
    cursor: pointer;
    transition: border-color 0.15s, background 0.15s;
}
.drop-zone.drag-over { border-color: #a0c8c8; background: #1a1a38; color: #a0c8c8; }

/* ── Description lines ─────────────────────────────────────────────────── */
.desc-line { display: flex; gap: 6px; align-items: center; margin-bottom: 4px; }
.desc-line input { flex: 1; }

/* ── Settings panel ────────────────────────────────────────────────────── */
.settings-overlay { position: fixed; inset: 0; background: rgba(0,0,0,0.5); z-index: 100; }
.settings-panel {
    position: fixed; top: 0; right: 0; bottom: 0; width: 320px;
    background: #16162a; border-left: 1px solid #2a2a4a;
    padding: 20px; z-index: 101; overflow-y: auto;
    display: flex; flex-direction: column; gap: 16px;
}
.settings-panel h2 { color: #a0c8c8; font-size: 16px; margin-bottom: 4px; }
.setting-row { display: flex; flex-direction: column; gap: 6px; }
.setting-row label { font-size: 11px; color: #7878a0; text-transform: uppercase; letter-spacing: 0.06em; }
.toggle-row { display: flex; align-items: center; justify-content: space-between; }
.toggle-row span { font-size: 13px; }
input[type="checkbox"] { width: 16px; height: 16px; cursor: pointer; accent-color: #a0c8c8; }

/* ── Confirm dialog ────────────────────────────────────────────────────── */
.dialog-overlay { position: fixed; inset: 0; background: rgba(0,0,0,0.6); z-index: 200; display: flex; align-items: center; justify-content: center; }
.dialog-box { background: #1e1e38; border: 1px solid #2a2a4a; border-radius: 8px; padding: 24px; max-width: 400px; width: 90%; }
.dialog-box p { margin-bottom: 16px; line-height: 1.5; }
.dialog-actions { display: flex; gap: 10px; justify-content: flex-end; }

/* ── Sliders ───────────────────────────────────────────────────────────── */
.slider-row { display: flex; align-items: center; gap: 8px; }
input[type="range"] { flex: 1; accent-color: #a0c8c8; }
.slider-val { min-width: 36px; text-align: right; font-size: 12px; color: #a0a0c0; }

/* ── Colour swatch ─────────────────────────────────────────────────────── */
.color-swatch { display: inline-block; width: 12px; height: 12px; border-radius: 2px; border: 1px solid #3a3a5a; vertical-align: middle; margin-right: 4px; }

/* ── Scrollbar ─────────────────────────────────────────────────────────── */
::-webkit-scrollbar { width: 6px; }
::-webkit-scrollbar-track { background: transparent; }
::-webkit-scrollbar-thumb { background: #2a2a4a; border-radius: 3px; }
::-webkit-scrollbar-thumb:hover { background: #3a3a6a; }
```

- [ ] **Step 2: Create wwwroot/js/dropzone.js**

```javascript
window.dropzoneInterop = {
    _handlers: {},

    init(elementId, dotnetRef) {
        const el = document.getElementById(elementId);
        if (!el) return;

        const onDragOver = e => { e.preventDefault(); el.classList.add('drag-over'); };
        const onDragLeave = () => el.classList.remove('drag-over');
        const onDrop = async e => {
            e.preventDefault();
            el.classList.remove('drag-over');
            const file = e.dataTransfer?.files?.[0];
            if (!file || !file.name.endsWith('.json')) return;
            const text = await file.text();
            dotnetRef.invokeMethodAsync('OnFileDropped', file.name, text);
        };

        el.addEventListener('dragover', onDragOver);
        el.addEventListener('dragleave', onDragLeave);
        el.addEventListener('drop', onDrop);
        this._handlers[elementId] = { onDragOver, onDragLeave, onDrop };
    },

    dispose(elementId) {
        const el = document.getElementById(elementId);
        const h = this._handlers[elementId];
        if (el && h) {
            el.removeEventListener('dragover', h.onDragOver);
            el.removeEventListener('dragleave', h.onDragLeave);
            el.removeEventListener('drop', h.onDrop);
        }
        delete this._handlers[elementId];
    }
};
```

- [ ] **Step 3: Register the JS file in index.html**

In `C.H.A.O. Mission Creator/wwwroot/index.html`, add before `</body>`:
```html
<script src="js/dropzone.js"></script>
```

- [ ] **Step 4: Build to verify**

Run: `dotnet build "C.H.A.O. Mission Creator/C.H.A.O. Mission Creator.csproj" -f net10.0-windows10.0.19041.0`
Expected: Build succeeded

- [ ] **Step 5: Commit**

```bash
git add "C.H.A.O. Mission Creator/wwwroot/"
git commit -m "feat: add app CSS layout and dropzone JS interop"
```

---

## Task 11: MainLayout + Header

**Files:**
- Replace: `C.H.A.O. Mission Creator/Components/Layout/MainLayout.razor`

- [ ] **Step 1: Replace MainLayout.razor**

```razor
@inherits LayoutComponentBase
@inject AppSettingsService Settings
@inject ColorReferenceService ColorSvc

<div class="app-shell">
    <header class="app-header">
        <span>≡ C.H.A.O. Mission Creator</span>
        <button class="icon-btn" @onclick="ToggleSettings" title="Settings">⚙</button>
    </header>

    <div class="main-area">
        @Body
    </div>

    @if (_showSettings)
    {
        <div class="settings-overlay" @onclick="ToggleSettings"></div>
        <SettingsPanel OnClose="ToggleSettings" />
    }
</div>

@code {
    private bool _showSettings;

    protected override async Task OnInitializedAsync()
    {
        await Settings.LoadAsync();
        await ColorSvc.EnsureLoadedAsync();
    }

    private void ToggleSettings() => _showSettings = !_showSettings;
}
```

- [ ] **Step 2: Build to verify**

Run: `dotnet build "C.H.A.O. Mission Creator/C.H.A.O. Mission Creator.csproj" -f net10.0-windows10.0.19041.0`
Expected: Build succeeded (SettingsPanel not yet created — expected compile error; move to next step first)

---

## Task 12: Shared Components (SettingsPanel, ConfirmDialog, SaveBar, DropZone)

**Files:**
- Create: `C.H.A.O. Mission Creator/Components/Shared/SettingsPanel.razor`
- Create: `C.H.A.O. Mission Creator/Components/Shared/ConfirmDialog.razor`
- Create: `C.H.A.O. Mission Creator/Components/Shared/SaveBar.razor`
- Create: `C.H.A.O. Mission Creator/Components/Shared/DropZone.razor`

- [ ] **Step 1: Create SettingsPanel.razor**

```razor
@inject AppSettingsService Settings

<div class="settings-panel">
    <h2>Settings</h2>

    <div class="setting-row">
        <label>SA2 Install Path</label>
        <div style="display:flex;gap:6px">
            <input type="text" style="flex:1" value="@Settings.Sa2Path"
                   @onchange="e => Settings.Sa2Path = e.Value?.ToString()" />
            <button class="btn btn-secondary" @onclick="BrowseAsync">Browse</button>
        </div>
    </div>

    <div class="setting-row">
        <div class="toggle-row">
            <span>Enable all checks</span>
            <input type="checkbox" checked="@Settings.EnableAllChecks"
                   @onchange="e => ToggleSetting(e, v => Settings.EnableAllChecks = v)"
                   title="Enables advanced checks such as character fear, distance, and meet count." />
        </div>
    </div>

    <div class="setting-row">
        <div class="toggle-row">
            <span>Enable Advanced Color Check</span>
            <input type="checkbox" checked="@Settings.EnableAdvancedColorCheck"
                   @onchange="e => ToggleSetting(e, v => Settings.EnableAdvancedColorCheck = v)"
                   title="Shows an Advanced Color Check button, allowing you to pick a single exact color." />
        </div>
    </div>

    <div style="margin-top:auto;display:flex;gap:8px">
        <button class="btn btn-primary" @onclick="SaveAndCloseAsync">Save</button>
        <button class="btn btn-secondary" @onclick="OnClose">Cancel</button>
    </div>
</div>

@code {
    [Parameter] public EventCallback OnClose { get; set; }

    private async Task BrowseAsync()
    {
        var picker = new Windows.Storage.Pickers.FolderPicker();
        picker.FileTypeFilter.Add("*");
        WinRT.Interop.InitializeWithWindow.Initialize(picker, GetHwnd());
        var folder = await picker.PickSingleFolderAsync();
        if (folder is not null)
        {
            Settings.Sa2Path = folder.Path;
            StateHasChanged();
        }
    }

    private async Task SaveAndCloseAsync()
    {
        await Settings.SaveAsync();
        await OnClose.InvokeAsync();
    }

    private void ToggleSetting(ChangeEventArgs e, Action<bool> setter)
    {
        if (e.Value is bool b) setter(b);
    }

    private static IntPtr GetHwnd()
    {
        var window = Application.Current?.Windows[0];
        if (window?.Handler?.PlatformView is Microsoft.UI.Xaml.Window w)
            return WinRT.Interop.WindowNative.GetWindowHandle(w);
        return IntPtr.Zero;
    }
}
```

- [ ] **Step 2: Create ConfirmDialog.razor**

```razor
<div class="dialog-overlay">
    <div class="dialog-box">
        <p>@Message</p>
        <div class="dialog-actions">
            <button class="btn btn-secondary" @onclick="OnCancel">Go back</button>
            <button class="btn btn-primary" @onclick="OnConfirm">Save anyway</button>
        </div>
    </div>
</div>

@code {
    [Parameter] public string Message { get; set; } = "";
    [Parameter] public EventCallback OnConfirm { get; set; }
    [Parameter] public EventCallback OnCancel { get; set; }
}
```

- [ ] **Step 3: Create SaveBar.razor**

```razor
<div class="save-bar">
    <button class="btn btn-primary" @onclick="OnSaveToMod">Save to Mod Folder</button>
    <button class="btn btn-secondary" @onclick="OnSaveAs">Save As…</button>
    <span class="status @StatusClass">@StatusText</span>
</div>

@code {
    [Parameter] public EventCallback OnSaveToMod { get; set; }
    [Parameter] public EventCallback OnSaveAs { get; set; }
    [Parameter] public string StatusText { get; set; } = "Ready";
    [Parameter] public string StatusClass { get; set; } = "";
}
```

- [ ] **Step 4: Create DropZone.razor**

```razor
@inject IJSRuntime JS
@implements IAsyncDisposable

<div id="@_id" class="drop-zone" @onclick="OnClick">
    <div>📂 Drop .json here</div>
    <div style="font-size:12px;margin-top:4px">or click to open</div>
</div>

@code {
    [Parameter] public EventCallback<(string FileName, string Content)> OnFilePicked { get; set; }

    private readonly string _id = $"dz-{Guid.NewGuid():N}";
    private DotNetObjectReference<DropZone>? _ref;

    protected override async Task OnAfterRenderAsync(bool firstRender)
    {
        if (firstRender)
        {
            _ref = DotNetObjectReference.Create(this);
            await JS.InvokeVoidAsync("dropzoneInterop.init", _id, _ref);
        }
    }

    [JSInvokable]
    public async Task OnFileDropped(string fileName, string content)
        => await OnFilePicked.InvokeAsync((fileName, content));

    private async Task OnClick()
    {
        var picker = new Windows.Storage.Pickers.FileOpenPicker();
        picker.FileTypeFilter.Add(".json");
        WinRT.Interop.InitializeWithWindow.Initialize(picker, GetHwnd());
        var file = await picker.PickSingleFileAsync();
        if (file is null) return;
        var content = await Windows.Storage.FileIO.ReadTextAsync(file);
        await OnFilePicked.InvokeAsync((file.Name, content));
    }

    private static IntPtr GetHwnd()
    {
        var window = Application.Current?.Windows[0];
        if (window?.Handler?.PlatformView is Microsoft.UI.Xaml.Window w)
            return WinRT.Interop.WindowNative.GetWindowHandle(w);
        return IntPtr.Zero;
    }

    public async ValueTask DisposeAsync()
    {
        await JS.InvokeVoidAsync("dropzoneInterop.dispose", _id);
        _ref?.Dispose();
    }
}
```

- [ ] **Step 5: Build to verify**

Run: `dotnet build "C.H.A.O. Mission Creator/C.H.A.O. Mission Creator.csproj" -f net10.0-windows10.0.19041.0`
Expected: Build succeeded

- [ ] **Step 6: Commit**

```bash
git add "C.H.A.O. Mission Creator/Components/"
git add "C.H.A.O. Mission Creator/wwwroot/"
git commit -m "feat: add MainLayout, SettingsPanel, ConfirmDialog, SaveBar, DropZone"
```

---

## Task 13: MissionInfo Component

**Files:**
- Create: `C.H.A.O. Mission Creator/Components/MissionInfo.razor`

Shows the mission name, description lines, and the drop zone. The drop zone is replaced by a "Load different file" link once a mission is loaded.

- [ ] **Step 1: Create MissionInfo.razor**

```razor
@inject MissionFileService FileService

<div class="left-panel">
    <div>
        <label>Mission Name</label>
        <input type="text" value="@Model.Name"
               @onchange="e => { Model.Name = e.Value?.ToString() ?? ""; OnChanged.InvokeAsync(); }" />
    </div>

    <div>
        <label>Description</label>
        @for (int i = 0; i < Model.DescriptionLines.Count; i++)
        {
            var idx = i;
            <div class="desc-line">
                <input type="text" value="@Model.DescriptionLines[idx]"
                       @onchange="e => { Model.DescriptionLines[idx] = e.Value?.ToString() ?? ""; OnChanged.InvokeAsync(); }" />
                <button class="btn btn-danger" @onclick="() => RemoveLine(idx)">✕</button>
            </div>
        }
        <button class="btn-add" @onclick="AddLine">+ Add line</button>
    </div>

    @if (!_missionLoaded)
    {
        <DropZone OnFilePicked="LoadFile" />
    }
    else
    {
        <div style="font-size:12px;color:#5050a0">
            <a href="#" style="color:#7878a0" @onclick:preventDefault @onclick="ResetAndPickNew">
                ↩ Load different file
            </a>
        </div>
    }
</div>

@code {
    [Parameter] public MissionEditorModel Model { get; set; } = new();
    [Parameter] public EventCallback OnChanged { get; set; }
    [Parameter] public EventCallback<string?> OnStatusChanged { get; set; }

    private bool _missionLoaded;

    private async Task LoadFile((string FileName, string Content) file)
    {
        try
        {
            var loaded = FileService.Load(file.Content);
            // Copy loaded values into existing model so parent state updates
            Model.Name = loaded.Name;
            Model.DescriptionLines = loaded.DescriptionLines;
            Model.Requirements = loaded.Requirements;
            Model.Rewards = loaded.Rewards;
            Model.BonusRequirements = loaded.BonusRequirements;
            Model.BonusRewards = loaded.BonusRewards;
            _missionLoaded = true;
            await OnChanged.InvokeAsync();
            await OnStatusChanged.InvokeAsync($"Loaded: {file.FileName}");
        }
        catch (Exception ex)
        {
            await OnStatusChanged.InvokeAsync($"Error loading file: {ex.Message}");
        }
    }

    private void AddLine()
    {
        Model.DescriptionLines.Add("");
        OnChanged.InvokeAsync();
    }

    private void RemoveLine(int idx)
    {
        Model.DescriptionLines.RemoveAt(idx);
        OnChanged.InvokeAsync();
    }

    private void ResetAndPickNew() => _missionLoaded = false;
}
```

Note: `MissionFileService.Load` needs a string overload (content rather than path). Update `MissionFileService.cs`:

```csharp
// Add this overload alongside the existing Load(string filePath):
public MissionEditorModel LoadFromContent(string jsonContent)
    => _serializer.Deserialize(jsonContent);
```

And update the call in MissionInfo:
```csharp
var loaded = FileService.LoadFromContent(file.Content);
```

- [ ] **Step 2: Build to verify**

Run: `dotnet build "C.H.A.O. Mission Creator/C.H.A.O. Mission Creator.csproj" -f net10.0-windows10.0.19041.0`
Expected: Build succeeded

- [ ] **Step 3: Commit**

```bash
git add "C.H.A.O. Mission Creator/Components/MissionInfo.razor"
git add "C.H.A.O. Mission Creator/Services/MissionFileService.cs"
git commit -m "feat: add MissionInfo left panel component"
```

---

## Task 14: CheckRow Component

**Files:**
- Create: `C.H.A.O. Mission Creator/Components/Standard/CheckRow.razor`

Renders the right fields for each of the 11 `ValueCheckType` categories. All checks with a value field also show a **Range mode** selector and **Inverted** toggle.

- [ ] **Step 1: Create CheckRow.razor**

```razor
@inject AppSettingsService Settings
@inject ColorReferenceService ColorSvc

<div class="check-row">
    @* Type selector — only shown when multiple check types available for this requirement type *@
    @if (AvailableCheckTypes.Count > 1)
    {
        <select value="@Check.CheckType" @onchange="OnCheckTypeChanged">
            @foreach (var ct in AvailableCheckTypes)
            {
                <option value="@ct.Key">@ct.Value</option>
            }
        </select>
    }

    @* ── Per-type fields ── *@
    @if (Check.CheckType == "ChaoTypeCheck")
    {
        <select value="@(Check.Value ?? "2")" @onchange="e => SetValue(e.Value?.ToString())">
            @foreach (ChaoType t in Enum.GetValues<ChaoType>())
            {
                <option value="@((int)t)">@t.ToString().Replace("_", " ")</option>
            }
        </select>
    }
    else if (Check.CheckType is "SwimFlyInfluenceCheck" or "RunPowerInfluenceCheck" or "AlignmentCheck" or "MagnitudeCheck")
    {
        @RenderFloatSlider()
    }
    else if (Check.CheckType is "LevelCheck" or "GradeCheck" or "PointsCheck")
    {
        <select value="@(Check.Skill ?? "ChaoSkill_Swim")" @onchange="e => { Check.Skill = e.Value?.ToString(); OnChanged.InvokeAsync(Check); }">
            @foreach (ChaoSkill s in Enum.GetValues<ChaoSkill>())
            {
                <option value="ChaoSkill_@s">@s</option>
            }
        </select>
        @if (Check.CheckType == "LevelCheck")  { @RenderIntInput(0, 99) }
        else if (Check.CheckType == "GradeCheck")
        {
            <select value="@(Check.Value ?? "0")" @onchange="e => SetValue(e.Value?.ToString())">
                @foreach (StatGrade g in Enum.GetValues<StatGrade>())
                { <option value="@((int)g)">@g</option> }
            </select>
        }
        else { @RenderIntInput(0, 4000) }
    }
    else if (Check.CheckType == "HappinessCheck")  { @RenderIntInput(0, 32767) }
    else if (Check.CheckType == "ColorCheck")       { @RenderColorCheck() }
    else if (Check.CheckType == "ToneCheck")
    {
        <select value="@(Check.Value ?? "0")" @onchange="e => SetValue(e.Value?.ToString())">
            @foreach (ChaoTone t in Enum.GetValues<ChaoTone>())
            { <option value="@((int)t)">@t</option> }
        </select>
    }
    else if (Check.CheckType == "ShinyCheck")
    {
        <select value="@(Check.Value ?? "0")" @onchange="e => SetValue(e.Value?.ToString())">
            @foreach (ChaoShiny s in Enum.GetValues<ChaoShiny>())
            { <option value="@((int)s)">@s</option> }
        </select>
    }
    else if (Check.CheckType == "TextureCheck")
    {
        <select value="@(Check.Value ?? "SA2BTexture_None")" @onchange="e => SetValue(e.Value?.ToString())">
            @foreach (SA2BTexture t in Enum.GetValues<SA2BTexture>())
            { <option value="SA2BTexture_@t">@t</option> }
        </select>
    }
    else if (Check.CheckType is "AnimalArmsCheck" or "AnimalEarsCheck" or "AnimalForeheadCheck"
                             or "AnimalHornCheck" or "AnimalLegsCheck" or "AnimalTailCheck"
                             or "AnimalWingsCheck" or "AnimalFaceCheck")
    {
        <select value="@(Check.Value ?? "0")" @onchange="e => SetValue(e.Value?.ToString())">
            @foreach (Al_Animal a in Enum.GetValues<Al_Animal>().Where(a => a != Al_Animal.None))
            { <option value="Al_Animal_@a">@a.ToString().Replace("CWE_", "[CWE] ")</option> }
        </select>
    }
    else if (Check.CheckType is "FlagCheck" or "TotalCheck")  { @RenderFlagOrTotal() }
    else if (Check.CheckType is "AgeCheck" or "ReincarnationsCheck" or "AmountWonCheck")
    {
        @RenderIntInput(0, 9999)
    }
    else if (Check.CheckType == "KarateRankCheck")
    {
        <select value="@(Check.Value ?? "0")" @onchange="e => SetValue(e.Value?.ToString())">
            @foreach (KarateRank r in Enum.GetValues<KarateRank>())
            { <option value="KarateRank_@r">@r.ToString().Replace("_", " ")</option> }
        </select>
    }
    else if (Check.CheckType is "CharacterLikeCheck" or "CharacterFearCheck"
                              or "CharacterDistanceCheck" or "CharacterMeetCheck")
    {
        <select value="@(Check.Character ?? "CharacterBond_Sonic")"
                @onchange="e => { Check.Character = e.Value?.ToString(); OnChanged.InvokeAsync(Check); }">
            @foreach (CharacterBondOrder c in Enum.GetValues<CharacterBondOrder>())
            { <option value="@c">@c.ToString().Replace("CharacterBond_", "")</option> }
        </select>
        @if (Check.CheckType == "CharacterLikeCheck") { @RenderIntInput(-100, 100) }
        else                                          { @RenderIntInput(0, 9999) }
    }

    @* ── Range mode (for non-enum-only checks) ── *@
    @if (ShowsRangeSelector)
    {
        <select value="@Check.RangeMode" @onchange="e => { Check.RangeMode = Enum.Parse<RangeMode>(e.Value!.ToString()!); OnChanged.InvokeAsync(Check); }">
            <option value="Exact">Exact</option>
            <option value="Min">Min</option>
            <option value="Max">Max</option>
            <option value="Range">Range</option>
        </select>
        @if (Check.RangeMode == RangeMode.Range)
        {
            <span style="font-size:11px;color:#7878a0">→</span>
            @RenderRangeMax()
        }
    }

    @* ── Inverted toggle ── *@
    <label style="font-size:11px;color:#7878a0;display:flex;align-items:center;gap:4px">
        <input type="checkbox" checked="@Check.Inverted"
               @onchange="e => { Check.Inverted = (bool)(e.Value ?? false); OnChanged.InvokeAsync(Check); }" />
        Inverted
    </label>

    <button class="btn btn-danger" @onclick="() => OnRemove.InvokeAsync(Check)">✕</button>
</div>

@code {
    [Parameter] public CheckModel Check { get; set; } = new();
    [Parameter] public RequirementType RequirementType { get; set; }
    [Parameter] public EventCallback<CheckModel> OnChanged { get; set; }
    [Parameter] public EventCallback<CheckModel> OnRemove { get; set; }

    private Dictionary<string, string> AvailableCheckTypes => RequirementType switch
    {
        RequirementType.TypeRequirement => new()
        {
            ["ChaoTypeCheck"] = "Chao Type",
            ["SwimFlyInfluenceCheck"] = "Swim ↔ Fly",
            ["RunPowerInfluenceCheck"] = "Run ↔ Power",
            ["AlignmentCheck"] = "Alignment",
            ["MagnitudeCheck"] = "Magnitude",
        },
        RequirementType.StatRequirement => new()
        {
            ["LevelCheck"] = "Level",
            ["GradeCheck"] = "Grade",
            ["PointsCheck"] = "Points",
            ["HappinessCheck"] = "Happiness",
        },
        RequirementType.AppearanceRequirement => new()
        {
            ["ColorCheck"] = "Color",
            ["ToneCheck"] = "Tone",
            ["ShinyCheck"] = "Shiny",
            ["TextureCheck"] = "Texture",
            ["AnimalArmsCheck"] = "Arms",
            ["AnimalEarsCheck"] = "Ears",
            ["AnimalForeheadCheck"] = "Forehead",
            ["AnimalHornCheck"] = "Horn",
            ["AnimalLegsCheck"] = "Legs",
            ["AnimalTailCheck"] = "Tail",
            ["AnimalWingsCheck"] = "Wings",
            ["AnimalFaceCheck"] = "Face",
        },
        RequirementType.AbilityRequirement => new()
        {
            ["FlagCheck"] = "Specific Ability",
            ["TotalCheck"] = "Total Abilities",
        },
        RequirementType.ToyRequirement => new()
        {
            ["FlagCheck"] = "Specific Toy",
            ["TotalCheck"] = "Total Toys",
        },
        RequirementType.ClassesRequirement => new()
        {
            ["FlagCheck"] = "Specific Class",
            ["TotalCheck"] = "Total Classes",
        },
        RequirementType.RaceRequirement => new()
        {
            ["FlagCheck"] = "Specific Medal",
            ["TotalCheck"] = "Total Medals",
        },
        RequirementType.KarateRequirement => new()
        {
            ["KarateRankCheck"] = "Rank",
            ["AmountWonCheck"] = "Matches Won",
        },
        RequirementType.LifetimeRequirement => new()
        {
            ["AgeCheck"] = "Age",
            ["ReincarnationsCheck"] = "Reincarnations",
        },
        RequirementType.BondRequirement => BuildBondTypes(),
        _ => new(),
    };

    private Dictionary<string, string> BuildBondTypes()
    {
        var d = new Dictionary<string, string> { ["CharacterLikeCheck"] = "Like" };
        if (Settings.EnableAllChecks)
        {
            d["CharacterFearCheck"] = "Fear";
            d["CharacterDistanceCheck"] = "Distance";
            d["CharacterMeetCheck"] = "Meet Count";
        }
        return d;
    }

    // Checks that use enum dropdowns only (no range/inverted)
    private bool ShowsRangeSelector => Check.CheckType is not
        ("ChaoTypeCheck" or "ToneCheck" or "ShinyCheck" or "TextureCheck" or
         "AnimalArmsCheck" or "AnimalEarsCheck" or "AnimalForeheadCheck" or "AnimalHornCheck" or
         "AnimalLegsCheck" or "AnimalTailCheck" or "AnimalWingsCheck" or "AnimalFaceCheck" or
         "ColorCheck" or "FlagCheck" or "KarateRankCheck" or
         "SwimFlyInfluenceCheck" or "RunPowerInfluenceCheck" or "AlignmentCheck" or "MagnitudeCheck");

    private void SetValue(string? val) { Check.Value = val; Check.MinValue = val; Check.MaxValue = val; OnChanged.InvokeAsync(Check); }

    private void OnCheckTypeChanged(ChangeEventArgs e)
    {
        Check.CheckType = e.Value?.ToString() ?? Check.CheckType;
        Check.Value = null; Check.MinValue = null; Check.MaxValue = null;
        Check.Skill = null; Check.Character = null; Check.ColorGroup = null;
        Check.RangeMode = RangeMode.Exact;
        OnChanged.InvokeAsync(Check);
    }

    private RenderFragment RenderIntInput(int min, int max) => __builder =>
    {
        var current = int.TryParse(Check.RangeMode == RangeMode.Max ? Check.MaxValue : Check.Value ?? Check.MinValue, out var v) ? v : min;
        __builder.OpenElement(0, "div");
        __builder.AddAttribute(1, "class", "slider-row");
        __builder.OpenElement(2, "input");
        __builder.AddAttribute(3, "type", "range");
        __builder.AddAttribute(4, "min", min);
        __builder.AddAttribute(5, "max", max);
        __builder.AddAttribute(6, "value", current);
        __builder.AddAttribute(7, "oninput", EventCallback.Factory.Create<ChangeEventArgs>(this, e =>
        {
            var s = e.Value?.ToString();
            if (Check.RangeMode == RangeMode.Max) Check.MaxValue = s;
            else if (Check.RangeMode == RangeMode.Min) Check.MinValue = s;
            else { Check.Value = s; Check.MinValue = s; }
            OnChanged.InvokeAsync(Check);
        }));
        __builder.CloseElement();
        __builder.OpenElement(8, "span");
        __builder.AddAttribute(9, "class", "slider-val");
        __builder.AddContent(10, current);
        __builder.CloseElement();
        __builder.CloseElement();
    };

    private RenderFragment RenderRangeMax() => __builder =>
    {
        var current = int.TryParse(Check.MaxValue, out var v) ? v : 0;
        __builder.OpenElement(0, "input");
        __builder.AddAttribute(1, "type", "number");
        __builder.AddAttribute(2, "style", "width:70px");
        __builder.AddAttribute(3, "value", current);
        __builder.AddAttribute(4, "onchange", EventCallback.Factory.Create<ChangeEventArgs>(this, e =>
        {
            Check.MaxValue = e.Value?.ToString();
            OnChanged.InvokeAsync(Check);
        }));
        __builder.CloseElement();
    };

    private RenderFragment RenderFloatSlider() => __builder =>
    {
        bool isAlign = Check.CheckType is "AlignmentCheck" or "SwimFlyInfluenceCheck" or "RunPowerInfluenceCheck";
        float fmin = isAlign ? -1.0f : 0.0f;
        float current = float.TryParse(Check.Value, System.Globalization.NumberStyles.Float, System.Globalization.CultureInfo.InvariantCulture, out var f) ? f : fmin;
        string label = Check.CheckType switch
        {
            "SwimFlyInfluenceCheck"  => current < 0 ? $"Swim ({current:F2})" : $"Fly ({current:F2})",
            "RunPowerInfluenceCheck" => current < 0 ? $"Run ({current:F2})" : $"Power ({current:F2})",
            "AlignmentCheck"         => current < -0.33f ? "Dark" : current > 0.33f ? "Hero" : "Neutral",
            "MagnitudeCheck"         => $"{current:F2}",
            _ => $"{current:F2}"
        };
        __builder.OpenElement(0, "div"); __builder.AddAttribute(1, "class", "slider-row");
        __builder.OpenElement(2, "input");
        __builder.AddAttribute(3, "type", "range");
        __builder.AddAttribute(4, "min", isAlign ? "-100" : "0");
        __builder.AddAttribute(5, "max", "100");
        __builder.AddAttribute(6, "value", (int)(current * 100));
        __builder.AddAttribute(7, "oninput", EventCallback.Factory.Create<ChangeEventArgs>(this, e =>
        {
            if (int.TryParse(e.Value?.ToString(), out var iv))
            {
                var fv = iv / 100.0f;
                Check.Value = fv.ToString("F2", System.Globalization.CultureInfo.InvariantCulture);
                OnChanged.InvokeAsync(Check);
            }
        }));
        __builder.CloseElement();
        __builder.OpenElement(8, "span"); __builder.AddAttribute(9, "class", "slider-val"); __builder.AddContent(10, label); __builder.CloseElement();
        __builder.CloseElement();
    };

    private RenderFragment RenderColorCheck() => __builder =>
    {
        __builder.OpenElement(0, "div");
        if (Check.ColorGroup is not null || !Settings.EnableAdvancedColorCheck)
        {
            // Group picker mode
            __builder.OpenElement(1, "select");
            __builder.AddAttribute(2, "value", Check.ColorGroup ?? ColorSvc.Groups.FirstOrDefault()?.Name ?? "");
            __builder.AddAttribute(3, "onchange", EventCallback.Factory.Create<ChangeEventArgs>(this, e =>
            {
                Check.ColorGroup = e.Value?.ToString();
                Check.Value = null;
                OnChanged.InvokeAsync(Check);
            }));
            foreach (var g in ColorSvc.Groups)
            {
                __builder.OpenElement(4, "option");
                __builder.AddAttribute(5, "value", g.Name);
                __builder.AddContent(6, g.Name);
                __builder.CloseElement();
            }
            __builder.CloseElement();

            if (Settings.EnableAdvancedColorCheck)
            {
                __builder.OpenElement(7, "button");
                __builder.AddAttribute(8, "class", "btn btn-secondary");
                __builder.AddAttribute(9, "style", "font-size:11px");
                __builder.AddAttribute(10, "title", "There are hundreds of color variants. Use this only if you need a very specific color.");
                __builder.AddAttribute(11, "onclick", EventCallback.Factory.Create(this, () =>
                {
                    Check.ColorGroup = null;
                    Check.Value = "ChaoColor_Normal";
                    OnChanged.InvokeAsync(Check);
                }));
                __builder.AddContent(12, "Advanced Color Check");
                __builder.CloseElement();
            }
        }
        else
        {
            // Advanced mode: individual color dropdown
            __builder.OpenElement(1, "select");
            __builder.AddAttribute(2, "value", Check.Value ?? "ChaoColor_Normal");
            __builder.AddAttribute(3, "onchange", EventCallback.Factory.Create<ChangeEventArgs>(this, e =>
            {
                Check.Value = e.Value?.ToString();
                OnChanged.InvokeAsync(Check);
            }));
            foreach (var c in ColorSvc.AllColors)
            {
                __builder.OpenElement(4, "option");
                __builder.AddAttribute(5, "value", c.EnumName);
                __builder.AddContent(6, c.EnumName.Replace("ChaoColor_", ""));
                __builder.CloseElement();
            }
            __builder.CloseElement();

            __builder.OpenElement(7, "button");
            __builder.AddAttribute(8, "class", "btn btn-secondary");
            __builder.AddAttribute(9, "style", "font-size:11px");
            __builder.AddAttribute(10, "onclick", EventCallback.Factory.Create(this, () =>
            {
                Check.ColorGroup = ColorSvc.GetGroupForEnum(Check.Value ?? "ChaoColor_Normal") ?? ColorSvc.Groups[0].Name;
                Check.Value = null;
                OnChanged.InvokeAsync(Check);
            }));
            __builder.AddContent(11, "← Group Mode");
            __builder.CloseElement();
        }
        __builder.CloseElement();
    };

    private RenderFragment RenderFlagOrTotal() => __builder =>
    {
        if (Check.CheckType == "TotalCheck")
        {
            int max = RequirementType switch
            {
                RequirementType.AbilityRequirement => 21,
                RequirementType.ToyRequirement => 10,
                RequirementType.ClassesRequirement => 23,
                RequirementType.RaceRequirement => 15,
                _ => 100
            };
            // Reuse RenderIntInput inline
            var frag = RenderIntInput(0, max);
            frag(__builder);
        }
        else
        {
            // FlagCheck — multi-select checkboxes rendered as a compact list
            __builder.OpenElement(0, "div");
            __builder.AddAttribute(1, "style", "display:flex;flex-wrap:wrap;gap:4px");
            var flags = GetFlagOptions();
            int current = int.TryParse(Check.Value, out var iv) ? iv : 0;
            foreach (var (label, bit) in flags)
            {
                bool isSet = (current & bit) != 0;
                __builder.OpenElement(2, "label");
                __builder.AddAttribute(3, "style", "font-size:11px;display:flex;align-items:center;gap:2px");
                __builder.OpenElement(4, "input");
                __builder.AddAttribute(5, "type", "checkbox");
                __builder.AddAttribute(6, "checked", isSet);
                __builder.AddAttribute(7, "onchange", EventCallback.Factory.Create<ChangeEventArgs>(this, e =>
                {
                    bool chk = (bool)(e.Value ?? false);
                    int v2 = int.TryParse(Check.Value, out var prev) ? prev : 0;
                    Check.Value = (chk ? v2 | bit : v2 & ~bit).ToString();
                    OnChanged.InvokeAsync(Check);
                }));
                __builder.CloseElement();
                __builder.AddContent(8, label);
                __builder.CloseElement();
            }
            __builder.CloseElement();
        }
    };

    private List<(string Label, int Bit)> GetFlagOptions() => RequirementType switch
    {
        RequirementType.AbilityRequirement => new()
        {
            ("Belly Slide", 1<<0), ("Roll Around", 1<<1), ("Backstroke", 1<<2),
            ("Hop Around", 1<<3), ("Wash Face", 1<<4), ("Dash", 1<<5),
            ("Roar", 1<<6), ("Sharpen Nails", 1<<7), ("Pound Chest", 1<<8),
            ("Strut", 1<<9), ("Whistle", 1<<10), ("Sit-ups", 1<<11),
            ("Fart", 1<<12), ("Somersault", 1<<13), ("Hum", 1<<14),
            ("Fiery Ball", 1<<15), ("Lose Parts", 1<<16), ("Lose Legs", 1<<17),
            ("Breathe Fire", 1<<18), ("Buck Feet", 1<<19), ("Wag Tail", 1<<20),
        },
        RequirementType.ToyRequirement => new()
        {
            ("Rattle", 1<<0), ("Car", 1<<1), ("Picture Book", 1<<2),
            ("Sonic Doll", 1<<4), ("Broomstick", 1<<5), ("Pogo Stick", 1<<7),
            ("Crayons", 1<<8), ("Bubble Wand", 1<<9), ("Shovel", 1<<10), ("Watering Can", 1<<11),
        },
        RequirementType.ClassesRequirement => new()
        {
            ("Drawing Lv1", 0x1), ("Drawing Lv2", 0x2), ("Drawing Lv3", 0x4), ("Drawing Lv4", 0x8), ("Drawing Lv5", 0x10),
            ("Shake Dance", 0x100), ("Spin Dance", 0x200), ("Step Dance", 0x400), ("Go-Go Dance", 0x800),
            ("Exercise", 0x1000),
            ("Song Lv1", 0x10000), ("Song Lv2", 0x20000), ("Song Lv3", 0x40000), ("Song Lv4", 0x80000), ("Song Lv5", 0x100000),
            ("Bell", 0x1000000), ("Castanets", 0x2000000), ("Cymbals", 0x4000000), ("Drum", 0x8000000),
            ("Flute", 0x10000000), ("Maracas", 0x20000000), ("Trumpet", 0x40000000), ("Tambourine", unchecked((int)0x80000000)),
        },
        RequirementType.RaceRequirement => new()
        {
            ("Challenge", 1<<0), ("Beginner", 1<<1), ("Hero", 1<<2), ("Dark", 1<<3),
            ("Aquamarine", 1<<4), ("Topaz", 1<<5), ("Peridot", 1<<6), ("Garnet", 1<<7),
            ("Onyx", 1<<8), ("Diamond", 1<<9), ("Pearl", 1<<10), ("Amethyst", 1<<11),
            ("Emerald", 1<<12), ("Ruby", 1<<13), ("Sapphire", 1<<14),
        },
        _ => new(),
    };
}
```

- [ ] **Step 2: Build to verify**

Run: `dotnet build "C.H.A.O. Mission Creator/C.H.A.O. Mission Creator.csproj" -f net10.0-windows10.0.19041.0`
Expected: Build succeeded

- [ ] **Step 3: Commit**

```bash
git add "C.H.A.O. Mission Creator/Components/Standard/CheckRow.razor"
git commit -m "feat: add CheckRow with all 11 requirement check types"
```

---

## Task 15: OrBranch + RequirementCard

**Files:**
- Create: `C.H.A.O. Mission Creator/Components/Standard/OrBranch.razor`
- Create: `C.H.A.O. Mission Creator/Components/Standard/RequirementCard.razor`

- [ ] **Step 1: Create OrBranch.razor**

```razor
<div class="or-branch">
    @for (int i = 0; i < Checks.Count; i++)
    {
        var idx = i;
        <CheckRow Check="Checks[idx]"
                  RequirementType="RequirementType"
                  OnChanged="c => OnChanged.InvokeAsync()"
                  OnRemove="c => RemoveCheck(idx)" />
    }
    <button class="btn-add" @onclick="AddCheck">+ Add condition</button>
</div>

@code {
    [Parameter] public List<CheckModel> Checks { get; set; } = new();
    [Parameter] public RequirementType RequirementType { get; set; }
    [Parameter] public EventCallback OnChanged { get; set; }
    [Parameter] public EventCallback OnRemove { get; set; }

    private string DefaultCheckType => RequirementType switch
    {
        RequirementType.TypeRequirement       => "ChaoTypeCheck",
        RequirementType.StatRequirement       => "LevelCheck",
        RequirementType.AppearanceRequirement => "ColorCheck",
        RequirementType.AbilityRequirement    => "FlagCheck",
        RequirementType.ToyRequirement        => "FlagCheck",
        RequirementType.ClassesRequirement    => "FlagCheck",
        RequirementType.RaceRequirement       => "FlagCheck",
        RequirementType.KarateRequirement     => "KarateRankCheck",
        RequirementType.LifetimeRequirement   => "AgeCheck",
        RequirementType.BondRequirement       => "CharacterLikeCheck",
        _ => "ChaoTypeCheck",
    };

    private void AddCheck()
    {
        Checks.Add(new CheckModel { CheckType = DefaultCheckType });
        OnChanged.InvokeAsync();
    }

    private void RemoveCheck(int idx)
    {
        if (Checks.Count > 1) Checks.RemoveAt(idx);
        OnChanged.InvokeAsync();
    }
}
```

- [ ] **Step 2: Create RequirementCard.razor**

```razor
<div class="card">
    <div class="card-header">
        <select value="@Requirement.Type" @onchange="OnTypeChanged" style="flex:1">
            @foreach (RequirementType t in Enum.GetValues<RequirementType>())
            {
                <option value="@t">@t.ToString().Replace("Requirement", "")</option>
            }
        </select>
        <button class="btn btn-danger" @onclick="() => OnRemove.InvokeAsync(Requirement)">✕</button>
    </div>

    <div style="margin-bottom:8px">
        <input type="text" placeholder="Description" style="width:100%"
               value="@Requirement.Description"
               @onchange="e => { Requirement.Description = e.Value?.ToString() ?? ""; OnChanged.InvokeAsync(); }" />
    </div>

    @for (int i = 0; i < Requirement.Checks.Count; i++)
    {
        var branchIdx = i;
        @if (branchIdx > 0)
        {
            <div class="or-separator">— OR —</div>
        }
        <div style="display:flex;align-items:flex-start;gap:4px">
            <OrBranch Checks="Requirement.Checks[branchIdx]"
                      RequirementType="Requirement.Type"
                      OnChanged="OnChanged"
                      OnRemove="() => RemoveBranch(branchIdx)" />
            @if (Requirement.Checks.Count > 1)
            {
                <button class="btn btn-danger" style="margin-top:4px"
                        @onclick="() => RemoveBranch(branchIdx)">✕</button>
            }
        </div>
    }

    <button class="btn-add" style="margin-top:6px" @onclick="AddBranch">+ Add OR branch</button>
</div>

@code {
    [Parameter] public RequirementModel Requirement { get; set; } = new();
    [Parameter] public EventCallback OnChanged { get; set; }
    [Parameter] public EventCallback<RequirementModel> OnRemove { get; set; }

    private void OnTypeChanged(ChangeEventArgs e)
    {
        if (Enum.TryParse<RequirementType>(e.Value?.ToString(), out var t))
        {
            Requirement.Type = t;
            // Reset checks since check types are requirement-type-specific
            Requirement.Checks = new() { new() { new() } };
            OnChanged.InvokeAsync();
        }
    }

    private void AddBranch()
    {
        Requirement.Checks.Add(new() { new() });
        OnChanged.InvokeAsync();
    }

    private void RemoveBranch(int idx)
    {
        if (Requirement.Checks.Count > 1) Requirement.Checks.RemoveAt(idx);
        OnChanged.InvokeAsync();
    }
}
```

- [ ] **Step 3: Build to verify**

Run: `dotnet build "C.H.A.O. Mission Creator/C.H.A.O. Mission Creator.csproj" -f net10.0-windows10.0.19041.0`
Expected: Build succeeded

- [ ] **Step 4: Commit**

```bash
git add "C.H.A.O. Mission Creator/Components/Standard/OrBranch.razor"
git add "C.H.A.O. Mission Creator/Components/Standard/RequirementCard.razor"
git commit -m "feat: add OrBranch and RequirementCard components"
```

---

## Task 16: RewardCard

**Files:**
- Create: `C.H.A.O. Mission Creator/Components/Standard/RewardCard.razor`

- [ ] **Step 1: Create RewardCard.razor**

```razor
<div class="card">
    <div class="card-header">
        <select value="@Reward.Type" @onchange="OnTypeChanged" style="width:140px">
            @foreach (RewardType t in Enum.GetValues<RewardType>())
            { <option value="@t">@t.ToString().Replace("Reward", "")</option> }
        </select>
        @if (Reward.Type != RewardType.ChaoReward)
        {
            <input type="number" min="1" value="@Reward.Amount" style="width:60px"
                   @onchange="e => { Reward.Amount = int.TryParse(e.Value?.ToString(), out var v) ? v : 1; OnChanged.InvokeAsync(); }" />
        }
        <button class="btn btn-danger" @onclick="() => OnRemove.InvokeAsync(Reward)">✕</button>
    </div>

    <div style="margin-bottom:8px">
        <input type="text" placeholder="Description" style="width:100%"
               value="@Reward.Description"
               @onchange="e => { Reward.Description = e.Value?.ToString() ?? ""; OnChanged.InvokeAsync(); }" />
    </div>

    @switch (Reward.Type)
    {
        case RewardType.RingReward:
            break;

        case RewardType.FruitReward:
            <select value="@(Reward.EnumValue ?? "SA2BFruit_ChaoGardenFruit")"
                    @onchange="e => { Reward.EnumValue = e.Value?.ToString(); OnChanged.InvokeAsync(); }">
                @foreach (SA2BFruit f in Enum.GetValues<SA2BFruit>().Where(f => f != SA2BFruit.None))
                { <option value="SA2BFruit_@f">@f.ToString().Replace("_", " ")</option> }
            </select>
            break;

        case RewardType.AnimalReward:
            <select value="@(Reward.EnumValue ?? "Al_Animal_Penguin")"
                    @onchange="e => { Reward.EnumValue = e.Value?.ToString(); OnChanged.InvokeAsync(); }">
                <optgroup label="SA2B">
                    @foreach (var a in Sa2bAnimals)
                    { <option value="Al_Animal_@a">@a.ToString()</option> }
                </optgroup>
                <optgroup label="SADX via CWE">
                    @foreach (var a in CweAnimals)
                    { <option value="Al_Animal_@a">@a.ToString().Replace("CWE_", "")</option> }
                </optgroup>
                <optgroup label="New CWE">
                    @foreach (var a in NewCweAnimals)
                    { <option value="Al_Animal_@a">@a.ToString().Replace("CWE_", "")</option> }
                </optgroup>
            </select>
            break;

        case RewardType.SeedReward:
            <select value="@(Reward.EnumValue ?? "ChaoSeed_StrongSeed")"
                    @onchange="e => { Reward.EnumValue = e.Value?.ToString(); OnChanged.InvokeAsync(); }">
                @foreach (ChaoSeed s in Enum.GetValues<ChaoSeed>().Where(s => s != ChaoSeed.None))
                { <option value="ChaoSeed_@s">@s.ToString().Replace("Seed", " Seed")</option> }
            </select>
            break;

        case RewardType.EggReward:
            <select value="@(Reward.EnumValue ?? "eCHAO_EGGS::Normal")"
                    @onchange="e => { Reward.EnumValue = e.Value?.ToString(); OnChanged.InvokeAsync(); }">
                @foreach (EChaoEggs eg in Enum.GetValues<EChaoEggs>())
                { <option value="eCHAO_EGGS::@eg">@eg.ToString().Replace("_", " ")</option> }
            </select>
            break;

        case RewardType.HatReward:
            <select value="@(Reward.EnumValue ?? "SA2BHat_None")"
                    @onchange="e => { Reward.EnumValue = e.Value?.ToString(); OnChanged.InvokeAsync(); }">
                <optgroup label="Named Hats">
                    @foreach (var h in NamedHats)
                    { <option value="SA2BHat_@h">@h.ToString().Replace("_", " ")</option> }
                </optgroup>
                <optgroup label="EggShell">
                    @foreach (var h in EggShellHats)
                    { <option value="SA2BHat_@h">@h.ToString().Replace("_", " ")</option> }
                </optgroup>
            </select>
            break;

        case RewardType.ChaoReward:
            @{
                var cv = Reward.ChaoValue ??= new();
            }
            <div style="display:grid;grid-template-columns:auto 1fr;gap:6px;align-items:center">
                <span style="font-size:11px;color:#7878a0">Name</span>
                <input type="text" maxlength="12" value="@cv.Name"
                       @onchange="e => { cv.Name = e.Value?.ToString() ?? ""; OnChanged.InvokeAsync(); }" />

                <span style="font-size:11px;color:#7878a0">Type</span>
                <select value="@cv.ChaoType" @onchange="e => { cv.ChaoType = e.Value?.ToString() ?? cv.ChaoType; OnChanged.InvokeAsync(); }">
                    @foreach (EChaoType t in Enum.GetValues<EChaoType>().Where(t => t >= EChaoType.Child))
                    { <option value="eCHAO_TYPE::@t">@t.ToString().Replace("_", " ")</option> }
                </select>

                <span style="font-size:11px;color:#7878a0">Color</span>
                <input type="text" placeholder="ChaoColor_Normal" value="@cv.Color"
                       @onchange="e => { cv.Color = e.Value?.ToString() ?? cv.Color; OnChanged.InvokeAsync(); }" />

                <span style="font-size:11px;color:#7878a0">Texture</span>
                <select value="@cv.Texture" @onchange="e => { cv.Texture = e.Value?.ToString() ?? cv.Texture; OnChanged.InvokeAsync(); }">
                    @foreach (SA2BTexture t in Enum.GetValues<SA2BTexture>())
                    { <option value="SA2BTexture_@t">@t</option> }
                </select>

                <span style="font-size:11px;color:#7878a0">Tone</span>
                <select value="@cv.Tone" @onchange="e => { cv.Tone = e.Value?.ToString() ?? cv.Tone; OnChanged.InvokeAsync(); }">
                    @foreach (ChaoTone t in Enum.GetValues<ChaoTone>())
                    { <option value="ChaoTone_@t">@t</option> }
                </select>

                <span style="font-size:11px;color:#7878a0">Shiny</span>
                <select value="@cv.Shiny" @onchange="e => { cv.Shiny = e.Value?.ToString() ?? cv.Shiny; OnChanged.InvokeAsync(); }">
                    @foreach (ChaoShiny s in Enum.GetValues<ChaoShiny>())
                    { <option value="ChaoShiny_@s">@s</option> }
                </select>
            </div>
            break;
    }
</div>

@code {
    [Parameter] public RewardModel Reward { get; set; } = new();
    [Parameter] public EventCallback OnChanged { get; set; }
    [Parameter] public EventCallback<RewardModel> OnRemove { get; set; }

    private static readonly Al_Animal[] Sa2bAnimals =
        Enum.GetValues<Al_Animal>().Where(a => a != Al_Animal.None && (int)a <= 20).ToArray();
    private static readonly Al_Animal[] CweAnimals =
        Enum.GetValues<Al_Animal>().Where(a => (int)a >= 25 && (int)a <= 39).ToArray();
    private static readonly Al_Animal[] NewCweAnimals =
        Enum.GetValues<Al_Animal>().Where(a => (int)a >= 40 && (int)a <= 42).ToArray();
    private static readonly SA2BHat[] NamedHats =
        Enum.GetValues<SA2BHat>().Where(h => (int)h <= 15).ToArray();
    private static readonly SA2BHat[] EggShellHats =
        Enum.GetValues<SA2BHat>().Where(h => (int)h >= 16).ToArray();

    private void OnTypeChanged(ChangeEventArgs e)
    {
        if (Enum.TryParse<RewardType>(e.Value?.ToString(), out var t))
        {
            Reward.Type = t;
            Reward.EnumValue = null;
            Reward.ChaoValue = t == RewardType.ChaoReward ? new() : null;
            OnChanged.InvokeAsync();
        }
    }
}
```

- [ ] **Step 2: Build to verify**

Run: `dotnet build "C.H.A.O. Mission Creator/C.H.A.O. Mission Creator.csproj" -f net10.0-windows10.0.19041.0`
Expected: Build succeeded

- [ ] **Step 3: Commit**

```bash
git add "C.H.A.O. Mission Creator/Components/Standard/RewardCard.razor"
git commit -m "feat: add RewardCard with all 7 reward types"
```

---

## Task 17: RequirementPanel + RewardPanel + StandardTab

**Files:**
- Create: `C.H.A.O. Mission Creator/Components/Standard/RequirementPanel.razor`
- Create: `C.H.A.O. Mission Creator/Components/Standard/RewardPanel.razor`
- Create: `C.H.A.O. Mission Creator/Components/Standard/StandardTab.razor`

- [ ] **Step 1: Create RequirementPanel.razor**

```razor
<div class="panel-col">
    <div class="panel-header">
        <span>Requirements</span>
        <button class="btn btn-primary" style="font-size:11px;padding:3px 10px" @onclick="AddRequirement">+ Add</button>
    </div>
    <div class="panel-scroll">
        @foreach (var req in Requirements)
        {
            <RequirementCard Requirement="req"
                             OnChanged="OnChanged"
                             OnRemove="r => { Requirements.Remove(r); OnChanged.InvokeAsync(); }" />
        }
    </div>
</div>

@code {
    [Parameter] public List<RequirementModel> Requirements { get; set; } = new();
    [Parameter] public EventCallback OnChanged { get; set; }

    private void AddRequirement()
    {
        Requirements.Add(new RequirementModel());
        OnChanged.InvokeAsync();
    }
}
```

- [ ] **Step 2: Create RewardPanel.razor**

```razor
<div class="panel-col">
    <div class="panel-header">
        <span>Rewards</span>
        <button class="btn btn-primary" style="font-size:11px;padding:3px 10px" @onclick="AddReward">+ Add</button>
    </div>
    <div class="panel-scroll">
        @foreach (var rew in Rewards)
        {
            <RewardCard Reward="rew"
                        OnChanged="OnChanged"
                        OnRemove="r => { Rewards.Remove(r); OnChanged.InvokeAsync(); }" />
        }
    </div>
</div>

@code {
    [Parameter] public List<RewardModel> Rewards { get; set; } = new();
    [Parameter] public EventCallback OnChanged { get; set; }

    private void AddReward()
    {
        Rewards.Add(new RewardModel());
        OnChanged.InvokeAsync();
    }
}
```

- [ ] **Step 3: Create StandardTab.razor**

```razor
<div class="tab-content">
    <RequirementPanel Requirements="Requirements" OnChanged="OnChanged" />
    <RewardPanel Rewards="Rewards" OnChanged="OnChanged" />
</div>

@code {
    [Parameter] public List<RequirementModel> Requirements { get; set; } = new();
    [Parameter] public List<RewardModel> Rewards { get; set; } = new();
    [Parameter] public EventCallback OnChanged { get; set; }
}
```

- [ ] **Step 4: Build to verify**

Run: `dotnet build "C.H.A.O. Mission Creator/C.H.A.O. Mission Creator.csproj" -f net10.0-windows10.0.19041.0`
Expected: Build succeeded

- [ ] **Step 5: Commit**

```bash
git add "C.H.A.O. Mission Creator/Components/Standard/"
git commit -m "feat: add RequirementPanel, RewardPanel, StandardTab"
```

---

## Task 18: BonusTab + EditorTabs

**Files:**
- Create: `C.H.A.O. Mission Creator/Components/Bonus/BonusTab.razor`
- Create: `C.H.A.O. Mission Creator/Components/EditorTabs.razor`

- [ ] **Step 1: Create BonusTab.razor**

Per spec: a single "+ Add Bonus" button at the bottom adds one entry to **each** list independently.

```razor
<div class="tab-content">
    <RequirementPanel Requirements="BonusRequirements" OnChanged="OnChanged" />
    <RewardPanel Rewards="BonusRewards" OnChanged="OnChanged" />
</div>
<div style="padding:8px 12px;border-top:1px solid #2a2a4a">
    <button class="btn btn-secondary" @onclick="AddBonus">+ Add Bonus Pair</button>
</div>

@code {
    [Parameter] public List<RequirementModel> BonusRequirements { get; set; } = new();
    [Parameter] public List<RewardModel> BonusRewards { get; set; } = new();
    [Parameter] public EventCallback OnChanged { get; set; }

    private void AddBonus()
    {
        BonusRequirements.Add(new RequirementModel());
        BonusRewards.Add(new RewardModel());
        OnChanged.InvokeAsync();
    }
}
```

- [ ] **Step 2: Create EditorTabs.razor**

```razor
<div class="right-panel">
    <div class="tab-bar">
        <button class="@(_activeTab == 0 ? "active" : "")" @onclick="() => _activeTab = 0">Standard</button>
        <button class="@(_activeTab == 1 ? "active" : "")" @onclick="() => _activeTab = 1">Bonus</button>
    </div>

    @if (_activeTab == 0)
    {
        <StandardTab Requirements="Model.Requirements"
                     Rewards="Model.Rewards"
                     OnChanged="OnChanged" />
    }
    else
    {
        <BonusTab BonusRequirements="Model.BonusRequirements"
                  BonusRewards="Model.BonusRewards"
                  OnChanged="OnChanged" />
    }
</div>

@code {
    [Parameter] public MissionEditorModel Model { get; set; } = new();
    [Parameter] public EventCallback OnChanged { get; set; }

    private int _activeTab;
}
```

- [ ] **Step 3: Build to verify**

Run: `dotnet build "C.H.A.O. Mission Creator/C.H.A.O. Mission Creator.csproj" -f net10.0-windows10.0.19041.0`
Expected: Build succeeded

- [ ] **Step 4: Commit**

```bash
git add "C.H.A.O. Mission Creator/Components/Bonus/BonusTab.razor"
git add "C.H.A.O. Mission Creator/Components/EditorTabs.razor"
git commit -m "feat: add BonusTab and EditorTabs"
```

---

## Task 19: MissionEditor Root Page

**Files:**
- Create: `C.H.A.O. Mission Creator/Components/Pages/MissionEditor.razor`

This is the root page. It owns the `MissionEditorModel`, wires save actions, and handles the bonus mismatch check before saving.

- [ ] **Step 1: Create MissionEditor.razor**

```razor
@page "/"
@inject AppSettingsService Settings
@inject MissionFileService FileService

<div class="main-area">
    <MissionInfo Model="_model"
                 OnChanged="StateHasChanged"
                 OnStatusChanged="SetStatus" />
    <EditorTabs Model="_model" OnChanged="StateHasChanged" />
</div>

<SaveBar OnSaveToMod="SaveToModAsync"
         OnSaveAs="SaveAsAsync"
         StatusText="@_statusText"
         StatusClass="@_statusClass" />

@if (_confirmMessage is not null)
{
    <ConfirmDialog Message="@_confirmMessage"
                   OnConfirm="() => ExecuteSave(_pendingSaveAction)"
                   OnCancel="CancelSave" />
}

@code {
    private readonly MissionEditorModel _model = new();
    private string _statusText = "Ready";
    private string _statusClass = "";
    private string? _confirmMessage;
    private Func<Task>? _pendingSaveAction;

    private async Task SaveToModAsync()
    {
        if (!CheckBonusMismatch(out var msg))
        {
            await DoSaveToModAsync();
            return;
        }
        _confirmMessage = msg;
        _pendingSaveAction = DoSaveToModAsync;
        StateHasChanged();
    }

    private async Task SaveAsAsync()
    {
        if (!CheckBonusMismatch(out var msg))
        {
            await DoSaveAsAsync();
            return;
        }
        _confirmMessage = msg;
        _pendingSaveAction = DoSaveAsAsync;
        StateHasChanged();
    }

    private async Task DoSaveToModAsync()
    {
        CancelSave();
        if (string.IsNullOrWhiteSpace(Settings.Sa2Path))
        {
            SetStatus("SA2 path not set — open Settings to configure it.", "status-err");
            return;
        }
        try
        {
            var path = await FileService.SaveToBacklogAsync(_model, Settings.Sa2Path);
            SetStatus($"Saved to Backlog: {path}", "status-ok");
        }
        catch (Exception ex)
        {
            SetStatus($"Save failed: {ex.Message}", "status-err");
        }
    }

    private async Task DoSaveAsAsync()
    {
        CancelSave();
        try
        {
            var path = await FileService.SaveAsAsync(_model);
            SetStatus(path is null ? "Save cancelled." : $"Saved: {path}", path is null ? "" : "status-ok");
        }
        catch (Exception ex)
        {
            SetStatus($"Save failed: {ex.Message}", "status-err");
        }
    }

    private async Task ExecuteSave(Func<Task>? action)
    {
        if (action is not null) await action();
    }

    private void CancelSave()
    {
        _confirmMessage = null;
        _pendingSaveAction = null;
        StateHasChanged();
    }

    private bool CheckBonusMismatch(out string message)
    {
        var rCount = _model.BonusRequirements.Count;
        var wCount = _model.BonusRewards.Count;
        if (rCount == wCount) { message = ""; return false; }

        if (rCount > wCount)
            message = $"You have more bonus requirements than bonus rewards. Players who clear all {rCount} bonus requirements will only receive {wCount} bonus rewards — the extra cleared requirements won't grant anything. Make sure the number of bonus requirements and bonus rewards are equal. Save anyway?";
        else
            message = $"Only {rCount} of the {wCount} bonus rewards can be given out — there aren't enough bonus requirements to unlock the rest. Make sure the number of bonus requirements and bonus rewards are equal. Save anyway?";

        return true;
    }

    private void SetStatus(string text, string cssClass = "")
    {
        _statusText = text;
        _statusClass = cssClass;
        StateHasChanged();
    }
}
```

- [ ] **Step 2: Update Routes.razor to remove NotFound page reference issues**

Verify `Components/Routes.razor` content is still correct (it references `Pages.NotFound` which must exist):
```razor
<Router AppAssembly="typeof(MauiProgram).Assembly" NotFoundPage="typeof(Pages.NotFound)">
    <Found Context="routeData">
        <RouteView RouteData="routeData" DefaultLayout="typeof(Layout.MainLayout)"/>
        <FocusOnNavigate RouteData="routeData" Selector="h1"/>
    </Found>
</Router>
```

- [ ] **Step 3: Update SaveBar to accept the save bar outside the main-area grid**

The `SaveBar` renders inside the `app-shell` grid's third row. Update `MainLayout.razor` to pass through `@Body` for the middle row only, and place `SaveBar` in the layout:

Actually the save bar is rendered by `MissionEditor.razor` via `@Body` expansion — the `app-shell` footer row is the `save-bar` div. To wire this properly, the `MainLayout` should expose a `SaveBarContent` section, or `MissionEditor` renders the save bar as a `<div class="save-bar">` that CSS positions correctly.

**Simpler approach:** Change `app-shell` to `grid-template-rows: 44px 1fr` (remove footer row) and render `SaveBar` as a normal component at the bottom of `MissionEditor`'s markup. The CSS `position: sticky; bottom: 0` on `.save-bar` will anchor it.

Update `app.css`:
```css
.app-shell {
    display: grid;
    grid-template-rows: 44px 1fr;  /* Remove the 48px footer row */
    height: 100vh;
    overflow: hidden;
}
.main-area {
    display: grid;
    grid-template-columns: 380px 1fr;
    grid-template-rows: 1fr 48px;  /* Add footer row here */
    overflow: hidden;
}
```

And in `MissionEditor.razor`, the `<SaveBar>` sits in the second row of `.main-area`.

Alternatively, keep the layout simple: render SaveBar inside a wrapper div that spans both columns:
```razor
<div class="main-area">
    <div style="display:contents">
        <MissionInfo ... />
        <EditorTabs ... />
    </div>
    <div style="grid-column:1/-1">
        <SaveBar ... />
    </div>
</div>
```

Update `app.css` `.main-area`:
```css
.main-area {
    display: grid;
    grid-template-columns: 380px 1fr;
    grid-template-rows: 1fr 48px;
    overflow: hidden;
}
```

- [ ] **Step 4: Build to verify**

Run: `dotnet build "C.H.A.O. Mission Creator/C.H.A.O. Mission Creator.csproj" -f net10.0-windows10.0.19041.0`
Expected: Build succeeded

- [ ] **Step 5: Commit**

```bash
git add "C.H.A.O. Mission Creator/Components/Pages/MissionEditor.razor"
git commit -m "feat: add MissionEditor root page — wires save, mismatch dialog, status"
```

---

## Task 20: Final Wiring + Smoke Test

**Goal:** Run the app, verify drag-drop loads ChaoFarmer.json, all fields populate, and save round-trips the file correctly.

- [ ] **Step 1: Add `@using` for Bonus namespace in _Imports.razor**

```razor
@using C.H.A.O._Mission_Creator.Components.Standard
@using C.H.A.O._Mission_Creator.Components.Bonus
@using C.H.A.O._Mission_Creator.Components.Shared
```

- [ ] **Step 2: Build in Release to catch any remaining errors**

Run: `dotnet build "C.H.A.O. Mission Creator/C.H.A.O. Mission Creator.csproj" -f net10.0-windows10.0.19041.0 -c Release`
Expected: Build succeeded, 0 errors

- [ ] **Step 3: Run the app and test the golden path**

```bash
dotnet run --project "C.H.A.O. Mission Creator/C.H.A.O. Mission Creator.csproj" -f net10.0-windows10.0.19041.0
```

Test checklist:
1. App opens with the split-panel layout and dark theme
2. Drag `C.H.A.O\Missions\InProgress\ChaoFarmer.json` onto the drop zone → fields populate
3. All 5 requirements show with correct types and descriptions
4. All 7 rewards show with correct types and values
5. Gorilla requirement shows two OR branches, each with 4 AND conditions
6. StatRequirement checks show Skill dropdowns
7. Standard/Bonus tab switching works
8. Click "Save As…" → file picker opens → save to desktop → verify JSON matches original structure
9. Click "⚙ Settings" → set SA2 path → "Save to Mod Folder" writes the file correctly
10. Add a bonus requirement without a matching reward → save → mismatch dialog appears

- [ ] **Step 4: Final commit**

```bash
git add "C.H.A.O. Mission Creator/"
git commit -m "feat: complete C.H.A.O. Mission Creator initial implementation"
```

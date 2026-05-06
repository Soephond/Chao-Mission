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

// eCHAO_EGGS is enum class in C++ — magic_enum serializes as "eCHAO_EGGS::Normal"
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
    Topaz = 64, Pearl = 65, Metal1 = 66, Metal2 = 67, Glass = 68,
}

// eCHAO_TYPE is enum class — magic_enum serializes as "eCHAO_TYPE::Child"
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

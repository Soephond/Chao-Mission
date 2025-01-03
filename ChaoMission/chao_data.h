#pragma once
#include "pch.h"

enum CharacterBondOrder {
	CharacterBond_Sonic = 0,
	CharacterBond_Shadow = 1,
	CharacterBond_Tails = 2,
	CharacterBond_Eggman = 3,
	CharacterBond_Knuckles = 4,
	CharacterBond_Rouge = 5,
};

enum MedalFlags : Uint16
{
	MedalFlags_None = 0,
	MedalFlags_Challenge = 1 << 0,
	MedalFlags_Beginner = 1 << 1,
	MedalFlags_Hero = 1 << 2,
	MedalFlags_Dark = 1 << 3,
	MedalFlags_Aquamarine = 1 << 4,
	MedalFlags_Topaz = 1 << 5,
	MedalFlags_Peridot = 1 << 6,
	MedalFlags_Garnet = 1 << 7,
	MedalFlags_Onyx = 1 << 8,
	MedalFlags_Diamond = 1 << 9,
	MedalFlags_Pearl = 1 << 10,
	MedalFlags_Amethyst = 1 << 11,
	MedalFlags_Emerald = 1 << 12,
	MedalFlags_Ruby = 1 << 13,
	MedalFlags_Sapphire = 1 << 14,
};

enum KarateRank : byte
{
	KarateRank_NoRank = 0,
	KarateRank_TenthKyu = 1,
	KarateRank_NinthKyu = 2,
	KarateRank_EightKyu = 3,
	KarateRank_SeventhKyu = 4,
	KarateRank_SixthKyu = 5,
	KarateRank_FifthKyu = 6,
	KarateRank_FourthKyu = 7,
	KarateRank_ThirdKyu = 8,
	KarateRank_SecondKyu = 9,
	KarateRank_FirstKyu = 10,
	KarateRank_FirstDegree = 11,
	KarateRank_SecondDegree = 12,
	KarateRank_ThirdDegree = 13,
	KarateRank_FourthDegree = 14,
	KarateRank_FifthDegree = 15,
	KarateRank_SixthDegree = 16,
	KarateRank_SeventhDegree = 17,
	KarateRank_EightDegree = 18,
	KarateRank_NinthDegree = 19,
	KarateRank_GrandMaster = 20
};

enum ChaoSkill: byte {
	ChaoSkill_Swim = 0,
	ChaoSkill_Fly = 1,
	ChaoSkill_Run = 2,
	ChaoSkill_Power = 3,
	ChaoSkill_Stamina = 4,
	ChaoSkill_Luck = 5,
	ChaoSkill_Intelligence = 6,
	ChaoSkill_Unknown = 7
};

enum StatGrade: byte {
	StatGrade_E = 0,
	StatGrade_D = 1,
	StatGrade_C = 2,
	StatGrade_B = 3,
	StatGrade_A = 4,
	StatGrade_S = 5,
	StatGrade_X = 6
};

enum ChaoShiny : byte
{
	ChaoShiny_None = 0,
	ChaoShiny_Bright = 1,
	ChaoShiny_Shiny = 2,
};

enum ChaoTone : byte {
	ChaoTone_MonoTone = 1,
	ChaoTone_TwoTone = 0
};

enum AnimalParts : byte {
	AnimalParts_Arms = 0,
	AnimalParts_Ears = 1,
	AnimalParts_Forehead = 2,
	AnimalParts_Horn = 3,
	AnimalParts_Legs = 4,
	AnimalParts_Tail = 5,
	AnimalParts_Wings = 6,
	AnimalParts_Face = 7
};

enum Al_Animal : byte {
	Al_Animal_None = 255,
	Al_Animal_Penguin = 0,
	Al_Animal_Seal = 1,
	Al_Animal_Otter = 2,
	Al_Animal_Rabbit = 3,
	Al_Animal_Cheetah = 4,
	Al_Animal_Warthog = 5,
	Al_Animal_Bear = 6,
	Al_Animal_Tiger = 7,
	Al_Animal_Gorilla = 8,
	Al_Animal_Peacock = 9,
	Al_Animal_Parrot = 10,
	Al_Animal_Condor = 11,
	Al_Animal_Skunk = 12,
	Al_Animal_Sheep = 13,
	Al_Animal_Raccoon = 14,
	Al_Animal_HalfFish = 15,
	Al_Animal_SkeletonDog = 16,
	Al_Animal_Bat = 17,
	Al_Animal_Dragon = 18,
	Al_Animal_Unicorn = 19,
	Al_Animal_Phoenix = 20,
	Al_Animal_YellowChaosDrive = 21,
	Al_Animal_GreenChaosDrive = 22,
	Al_Animal_RedChaosDrive = 23,
	Al_Animal_PurpleChaosDrive = 24,
	Al_Animal_CWE_Seal = 25,
	Al_Animal_CWE_Penguin = 26,
	Al_Animal_CWE_Otter = 27,
	Al_Animal_CWE_Peacock = 28,
	Al_Animal_CWE_Swallow = 29,
	Al_Animal_CWE_Parrot = 30,
	Al_Animal_CWE_Deer = 31,
	Al_Animal_CWE_Rabbit = 32,
	Al_Animal_CWE_Kangaroo = 33,
	Al_Animal_CWE_Gorilla = 34,
	Al_Animal_CWE_Lion = 35,
	Al_Animal_CWE_Elephant = 36,
	Al_Animal_CWE_Mole = 37,
	Al_Animal_CWE_Koala = 38,
	Al_Animal_CWE_Skunk = 39,
	Al_Animal_CWE_Bee = 40,
	Al_Animal_CWE_Ladybug = 41,
	Al_Animal_CWE_Scorpion = 42,
};

enum ChaoColor: byte {
	ChaoColor_Normal = 0,
	ChaoColor_Yellow = 1,
	ChaoColor_White = 2,
	ChaoColor_Brown = 3,
	ChaoColor_SkyBlue = 4,
	ChaoColor_Pink = 5,
	ChaoColor_Blue = 6,
	ChaoColor_Grey = 7,
	ChaoColor_Green = 8,
	ChaoColor_Red = 9,
	ChaoColor_LimeGreen = 10,
	ChaoColor_Purple = 11,
	ChaoColor_Orange = 12,
	ChaoColor_Black = 13,
	ChaoColor_PowderBlue = 21,
	ChaoColor_DeepDarkBlue = 82,
	ChaoColor_DarkerGrey = 30,
	ChaoColor_IndianRed = 133,
	ChaoColor_LightCoral = 140,
	ChaoColor_Salmon = 177,
	ChaoColor_DarkSalmon = 237,
	ChaoColor_Crimson = 157,
	ChaoColor_PinkV2 = 203,
	ChaoColor_LightPink = 217,
	ChaoColor_HotPink = 151,
	ChaoColor_MediumVioletRed = 211,
	ChaoColor_PaleVioletRed = 187,
	ChaoColor_LightVioletRed = 213,
	ChaoColor_VioletRed = 226,
	ChaoColor_PaleGoldenrod = 193,
	ChaoColor_Thistle = 247,
	ChaoColor_Plum = 171,
	ChaoColor_Violet = 202,
	ChaoColor_Orchid = 238,
	ChaoColor_Magenta = 248,
	ChaoColor_MediumOrchid = 176,
	ChaoColor_MediumPurple = 165,
	ChaoColor_BlueViolet = 196,
	ChaoColor_DarkViolet = 191,
	ChaoColor_DarkMagenta = 229,
	ChaoColor_PurpleV2 = 17,
	ChaoColor_DarkPurpleV2 = 145,
	ChaoColor_DarkSlateBlueV2 = 153,
	ChaoColor_SlateBlue = 146,
	ChaoColor_GreenYellow = 224,
	ChaoColor_LawnGreenV2 = 215,
	ChaoColor_PaleLimeGreen = 220,
	ChaoColor_LimeGreenV2 = 239,
	ChaoColor_LightPaleGreen = 189,
	ChaoColor_PaleGreen = 205,
	ChaoColor_LightGreen = 147,
	ChaoColor_PaleSpringGreen = 250,
	ChaoColor_LightSpringGreen = 253,
	ChaoColor_SpringGreenV2 = 139,
	ChaoColor_MediumSeaGreen = 163,
	ChaoColor_SeaGreen = 128,
	ChaoColor_ForestGreen = 169,
	ChaoColor_GreenV2 = 200,
	ChaoColor_DarkGreen = 219,
	ChaoColor_OliveDrabV2 = 183,
	ChaoColor_DarkOliveDra = 231,
	ChaoColor_DarkOlive = 178,
	ChaoColor_DarkOliveGreen = 123,
	ChaoColor_DarkOliveGreenGrey = 184,
	ChaoColor_MediumAquamarine = 223,
	ChaoColor_Darkseagreen = 207,
	ChaoColor_PaleTurquoise = 143,
	ChaoColor_PaleAquamarine = 241,
	ChaoColor_LightAquamarine = 181,
	ChaoColor_Aquamarine = 175,
	ChaoColor_Turquoise = 212,
	ChaoColor_MediumTurquoiseV2 = 199,
	ChaoColor_SteelBlueV2 = 235,
	ChaoColor_PowderBlueV2 = 120,
	ChaoColor_CornflowerBlueV3 = 236,
	ChaoColor_NavyBlue = 251,
	ChaoColor_Wheat = 152,
	ChaoColor_Burlywood = 155,
	ChaoColor_Tan = 149,
	ChaoColor_RosyBrown = 214,
	ChaoColor_MediumGoldenrod = 131,
	ChaoColor_Goldenrod = 190,
	ChaoColor_DarkGoldenrodV2 = 232,
	ChaoColor_Peru = 244,
	ChaoColor_DarkGrey = 127,
	ChaoColor_Invisible1 = 14,
	ChaoColor_Invisible2 = 15,
	ChaoColor_FieldDrab = 16,
	ChaoColor_VividGold = 18,
	ChaoColor_AliceBlue = 19,
	ChaoColor_Sepia = 20,
	ChaoColor_RobinsEgg = 22,
	ChaoColor_PersianRose = 23,
	ChaoColor_RoyalBlue = 24,
	ChaoColor_StrongGreen = 25,
	ChaoColor_VenetianRed = 26,
	ChaoColor_SpringGreen = 27,
	ChaoColor_ElectricViolet = 28,
	ChaoColor_DeepCarrotOrange = 31,
	ChaoColor_LuckyYellow = 32,
	ChaoColor_FieldDrabV2 = 33,
	ChaoColor_AtollCyan = 34,
	ChaoColor_MauveTaupe = 35,
	ChaoColor_DarkSlateBlue = 36,
	ChaoColor_DimGrey1 = 37,
	ChaoColor_SanFelix = 38,
	ChaoColor_FaluRed = 39,
	ChaoColor_OliveDrab = 40,
	ChaoColor_PalatinatePurple = 41,
	ChaoColor_Russet = 42,
	ChaoColor_EagleGreyV2 = 43,
	ChaoColor_Limerick = 44,
	ChaoColor_DarkTan = 45,
	ChaoColor_Verdigris = 46,
	ChaoColor_Bouquet = 47,
	ChaoColor_WaikawaGrey = 48,
	ChaoColor_FruitSalad = 49,
	ChaoColor_Sunset = 50,
	ChaoColor_Celery = 51,
	ChaoColor_VividViolet = 52,
	ChaoColor_Tussock = 53,
	ChaoColor_DimGrey2 = 54,
	ChaoColor_GoldenBrown = 55,
	ChaoColor_PastelGreen = 56,
	ChaoColor_HarvestGold = 57,
	ChaoColor_Avocado = 58,
	ChaoColor_AppleGreen = 59,
	ChaoColor_Fulvous = 60,
	ChaoColor_ElectricLime = 61,
	ChaoColor_SeaPink = 62,
	ChaoColor_GoldenPoppy = 63,
	ChaoColor_ChateauGreen = 64,
	ChaoColor_Contessa = 65,
	ChaoColor_Fedora = 66,
	ChaoColor_NapierGreen = 67,
	ChaoColor_Magohany = 68,
	ChaoColor_Citrus = 69,
	ChaoColor_Rouge = 70,
	ChaoColor_Ginger = 71,
	ChaoColor_Neptune = 72,
	ChaoColor_Pelorus = 73,
	ChaoColor_ShamrockGreen = 74,
	ChaoColor_Sandstone = 75,
	ChaoColor_ParisGreen = 76,
	ChaoColor_RichBlue = 77,
	ChaoColor_Asparagus = 78,
	ChaoColor_DeepLilac = 79,
	ChaoColor_Finch = 80,
	ChaoColor_Cherry = 81,
	ChaoColor_VegasGold = 83,
	ChaoColor_DeepFuchsia = 84,
	ChaoColor_SalmonV3 = 85,
	ChaoColor_BlueStone = 86,
	ChaoColor_DarkPurple = 87,
	ChaoColor_Amulet = 88,
	ChaoColor_BlueVioletV2 = 89,
	ChaoColor_TurkishRose = 90,
	ChaoColor_BrownV2 = 91,
	ChaoColor_KellyGreen = 92,
	ChaoColor_ButterflyBush = 93,
	ChaoColor_HeartGold = 94,
	ChaoColor_DarkGoldenrod = 95,
	ChaoColor_VividCerise = 96,
	ChaoColor_Tangelo = 97,
	ChaoColor_BrandyRose = 98,
	ChaoColor_Citrine = 99,
	ChaoColor_Cranberry = 100,
	ChaoColor_SuluYellow = 101,
	ChaoColor_FringyFlower = 102,
	ChaoColor_GreenSmoke = 103,
	ChaoColor_MediumTurquoise = 104,
	ChaoColor_ThistleV2 = 105,
	ChaoColor_JordyBlue = 106,
	ChaoColor_Opal = 107,
	ChaoColor_OceanGreen = 108,
	ChaoColor_CoralTree = 109,
	ChaoColor_PistachioGreen = 110,
	ChaoColor_BrightLavender = 111,
	ChaoColor_Straw = 112,
	ChaoColor_Juniper = 113,
	ChaoColor_SchoonerYellow = 114,
	ChaoColor_MoodyBlue = 115,
	ChaoColor_Meteorite = 116,
	ChaoColor_MediumPersianBlue = 117,
	ChaoColor_RoyalPurple = 118,
	ChaoColor_PalatinateBlue = 119,
	ChaoColor_GovernorBay = 121,
	ChaoColor_BlueLagoon = 122,
	ChaoColor_PatriarchPurple = 124,
	ChaoColor_CuttySark = 125,
	ChaoColor_PurpleHeart = 126,
	ChaoColor_Cosmic = 129,
	ChaoColor_EgyptianBlue = 130,
	ChaoColor_UnitedNationBlue = 132,
	ChaoColor_CoralV2 = 134,
	ChaoColor_CornflowerBlue = 135,
	ChaoColor_Milan = 136,
	ChaoColor_CornflowerBlueV2 = 137,
	ChaoColor_SalmonV2 = 138,
	ChaoColor_Heliotrope = 141,
	ChaoColor_Downy = 142,
	ChaoColor_HeliotropeV2 = 144,
	ChaoColor_SelectiveYellow = 148,
	ChaoColor_RadicalRed = 150,
	ChaoColor_CarnationPink = 154,
	ChaoColor_Amber = 156,
	ChaoColor_PastelYellow = 158,
	ChaoColor_YumaYellow = 159,
	ChaoColor_AquamarineV2 = 160,
	ChaoColor_TeaRose = 161,
	ChaoColor_BlueBell = 162,
	ChaoColor_EagleGrey = 164,
	ChaoColor_MantisGreen = 166,
	ChaoColor_TerraCotta = 167,
	ChaoColor_InchWormGreen = 168,
	ChaoColor_SkyMagenta = 170,
	ChaoColor_LemonGrass = 172,
	ChaoColor_Transparent = 173,
	ChaoColor_ChartreuseYellow = 174,
	ChaoColor_CornYellow = 179,
	ChaoColor_Cream = 180,
	ChaoColor_LavenderBlush = 182,
	ChaoColor_Panache = 185,
	ChaoColor_DeepTangelo = 186,
	ChaoColor_Mahogany = 188,
	ChaoColor_StrongOrange = 192,
	ChaoColor_MediumSpringGreen = 194,
	ChaoColor_DeepSkyBlue = 195,
	ChaoColor_ElectricCyan = 197,
	ChaoColor_FrenchRose = 198,
	ChaoColor_PurplePizzazz = 201,
	ChaoColor_LightMagenta = 204,
	ChaoColor_StrongBlue = 206,
	ChaoColor_BleuDeFrance = 208,
	ChaoColor_CobaltBlue = 209,
	ChaoColor_SlateGrey = 210,
	ChaoColor_RiverBed = 216,
	ChaoColor_DeepSpringBud = 218,
	ChaoColor_PigmentGreen = 221,
	ChaoColor_IndiaGreen = 222,
	ChaoColor_AmericanRose = 225,
	ChaoColor_Scarlet = 227,
	ChaoColor_VividRed = 228,
	ChaoColor_Harlequin = 230,
	ChaoColor_Malachite = 233,
	ChaoColor_LawnGreen = 234,
	ChaoColor_ElectricIndigo = 240,
	ChaoColor_RedViolet = 242,
	ChaoColor_DeepMagenta = 243,
	ChaoColor_VividVermilion = 245,
	ChaoColor_PortlandOrange = 246,
	ChaoColor_VividOrange = 249,
	ChaoColor_VeryDarkGrey = 252,
	ChaoColor_Onyx = 254,
	ChaoColor_Nero = 255,
	ChaoColor_WhiteV2 = 29
};
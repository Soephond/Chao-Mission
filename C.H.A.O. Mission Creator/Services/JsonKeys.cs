namespace C.H.A.O._Mission_Creator.Services;

// All JSON field names used by MissionSerializer in one place.
// Change a key here and it propagates to both read and write paths.
internal static class JsonKeys
{
    // Top-level mission fields
    public const string MissionName        = "Name";
    public const string Description        = "Description";
    public const string Requirements       = "Requirements";
    public const string Rewards            = "Rewards";
    public const string BonusRequirements  = "Bonus Requirements";
    public const string BonusRewards       = "Bonus Rewards";

    // Requirement / check shared fields
    public const string Type               = "Type";
    public const string Checks             = "Checks";

    // Check value fields
    public const string Value              = "Value";
    public const string MinValue           = "MinValue";
    public const string MaxValue           = "MaxValue";
    public const string Inverted           = "Inverted";
    public const string Skill              = "Skill";
    public const string Character          = "Character";

    // Reward fields
    public const string Amount             = "Amount";

    // ChaoReward Value sub-object fields
    public const string ChaoType           = "ChaoType";
    public const string Color              = "Color";
    public const string Texture            = "Texture";
    public const string Tone               = "Tone";
    public const string Shiny              = "Shiny";
    public const string ChaoName           = "Name";

    // Prefixes for enums whose C# names lack the game prefix used in JSON
    public const string SkillPrefix        = "ChaoSkill_";
    public const string TexturePrefix      = "SA2BTexture_";
    public const string TonePrefix         = "ChaoTone_";
    public const string ShinyPrefix        = "ChaoShiny_";
    public const string ColorPrefix        = "ChaoColor_";
    public const string AnimalPrefix       = "Al_Animal_";
    public const string KarateRankPrefix   = "KarateRank_";
    public const string FruitPrefix        = "SA2BFruit_";
    public const string SeedPrefix         = "ChaoSeed_";
    public const string HatPrefix          = "SA2BHat_";
    // eCHAO_EGGS is enum class — magic_enum returns bare member name ("Normal", "Gold"), no prefix
}

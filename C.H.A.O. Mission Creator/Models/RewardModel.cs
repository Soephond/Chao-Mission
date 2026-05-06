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
    // EChaoType is enum class in C++ — magic_enum serializes as plain member name e.g. "Child"
    public EChaoType ChaoType { get; set; } = ModelDefaults.ChaoType;

    // ChaoColor has no C# enum (hundreds of values loaded from JSON at runtime).
    // Stored as the full enum name string e.g. "ChaoColor_Normal".
    public string Color { get; set; } = ModelDefaults.NormalColorEnumName;

    public SA2BTexture Texture { get; set; } = ModelDefaults.Texture;
    public ChaoTone    Tone    { get; set; } = ModelDefaults.Tone;
    public ChaoShiny   Shiny   { get; set; } = ModelDefaults.Shiny;
    public string      Name    { get; set; } = "";
}

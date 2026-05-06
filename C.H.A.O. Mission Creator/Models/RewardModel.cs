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
    // eCHAO_TYPE is enum class — magic_enum returns plain member name, e.g. "Child"
    public string ChaoType { get; set; } = "Child";
    // Regular enums keep their prefix, e.g. "ChaoColor_Normal", "SA2BTexture_None"
    public string Color { get; set; } = "ChaoColor_Normal";
    public string Texture { get; set; } = "SA2BTexture_None";
    public string Tone { get; set; } = "ChaoTone_MonoTone";
    public string Shiny { get; set; } = "ChaoShiny_None";
    public string Name { get; set; } = "";
}

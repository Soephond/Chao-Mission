namespace C.H.A.O._Mission_Creator.Models;

/// <summary>
/// Canonical default values for the editor models.
/// Change here to propagate everywhere — no magic strings scattered through the codebase.
/// </summary>
public static class ModelDefaults
{
    // ChaoRewardValue defaults
    public static readonly EChaoType    ChaoType = EChaoType.Child;
    public static readonly SA2BTexture  Texture  = SA2BTexture.None;
    public static readonly ChaoTone     Tone     = ChaoTone.MonoTone;
    public static readonly ChaoShiny    Shiny    = ChaoShiny.None;

    // ChaoColor has no C# enum (hundreds of entries loaded from JSON at runtime).
    // "Normal" is the first and most common color; prefix matches the game's enum naming.
    public const string NormalColorEnumName = "ChaoColor_Normal";

    // CheckModel defaults
    public static readonly ChaoSkill         DefaultSkill     = ChaoSkill.Swim;
    public static readonly CharacterBondOrder DefaultCharacter = CharacterBondOrder.CharacterBond_Sonic;
    public static readonly ChaoType          DefaultChaoType  = Models.ChaoType.Child;
    public static readonly StatGrade         DefaultGrade     = StatGrade.E;
    public static readonly ChaoTone          DefaultTone      = ChaoTone.TwoTone;
    public static readonly ChaoShiny         DefaultShiny     = ChaoShiny.None;
    public static readonly SA2BTexture       DefaultTexture   = SA2BTexture.None;
    public static readonly Al_Animal         DefaultAnimal    = Al_Animal.Penguin;
    public static readonly KarateRank        DefaultKarateRank = KarateRank.NoRank;
}
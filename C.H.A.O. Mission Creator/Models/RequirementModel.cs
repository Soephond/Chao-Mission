namespace C.H.A.O._Mission_Creator.Models;

public class RequirementModel
{
    public RequirementType Type { get; set; } = RequirementType.TypeRequirement;
    public string Description { get; set; } = "";
    // Outer list = OR branches; inner list = AND conditions within a branch
    public List<List<CheckModel>> Checks { get; set; } = new() { new() { new CheckModel() } };
}

public class CheckModel
{
    public ValueCheckType CheckType { get; set; } = ValueCheckType.ChaoTypeCheck;
    public RangeMode RangeMode { get; set; } = RangeMode.Exact;
    public bool Inverted { get; set; }

    // Enum name string or numeric string for Value/MinValue/MaxValue
    public string? Value { get; set; }
    public string? MinValue { get; set; }
    public string? MaxValue { get; set; }

    // For stat checks (LevelCheck, GradeCheck, PointsCheck)
    public ChaoSkill? Skill { get; set; }

    // For bond checks
    public CharacterBondOrder? Character { get; set; }

    // For ColorCheck group mode: null = advanced (exact color), set = group picker
    public ColorGroupName? ColorGroup { get; set; }
}
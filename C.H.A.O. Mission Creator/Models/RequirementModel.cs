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
    // Matches ValueCheckType enum name e.g. "LevelCheck"
    public string CheckType { get; set; } = nameof(ValueCheckType.ChaoTypeCheck);
    public RangeMode RangeMode { get; set; } = RangeMode.Exact;
    public bool Inverted { get; set; }

    // Enum name string or numeric string for Value/MinValue/MaxValue
    public string? Value { get; set; }
    public string? MinValue { get; set; }
    public string? MaxValue { get; set; }

    // For stat/happiness checks
    public string? Skill { get; set; }

    // For bond checks
    public string? Character { get; set; }

    // For ColorCheck group mode (non-advanced): group name e.g. "Red"
    // When set, serializer expands to one OR branch per color in group.
    // When null, Value holds the exact enum name (advanced mode).
    public string? ColorGroup { get; set; }
}
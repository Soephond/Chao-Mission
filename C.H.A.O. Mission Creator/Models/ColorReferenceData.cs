namespace C.H.A.O._Mission_Creator.Models;

public record ColorEntry(string EnumName, int Value, string Hex, ColorGroupName Group);

public record ColorGroup(ColorGroupName Name, IReadOnlyList<ColorEntry> Colors);
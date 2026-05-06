namespace C.H.A.O._Mission_Creator.Models;

public record ColorEntry(string EnumName, int Value, string Hex, string Group);

public record ColorGroup(string Name, string RepresentativeHex, IReadOnlyList<ColorEntry> Colors);

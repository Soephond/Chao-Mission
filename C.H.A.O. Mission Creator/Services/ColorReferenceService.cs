using System.Text.Json;
using C.H.A.O._Mission_Creator.Models;

namespace C.H.A.O._Mission_Creator.Services;

public class ColorReferenceService
{
    private List<ColorEntry>? _entries;
    private List<ColorGroup>? _groups;

    public async Task EnsureLoadedAsync()
    {
        if (_entries is not null) return;

        await using var stream = await FileSystem.OpenAppPackageFileAsync("chao_colors_reference.json");
        using var doc = await JsonDocument.ParseAsync(stream);
        var arr = doc.RootElement.GetProperty("colors");

        _entries = new List<ColorEntry>();
        foreach (var el in arr.EnumerateArray())
        {
            _entries.Add(new ColorEntry(
                el.GetProperty("enum_name").GetString()!,
                el.GetProperty("value").GetInt32(),
                el.GetProperty("hex").GetString()!,
                el.GetProperty("group").GetString()!
            ));
        }

        var groupOrder = new[] {
            "Normal","Red","Orange","Yellow","Lime Green","Green",
            "Powder Blue","Sky Blue","Blue","Dark Blue","Purple",
            "Pink","White","Grey","Black","Brown"
        };
        var repHex = new Dictionary<string, string> {
            ["Normal"]="#A0C8C8", ["Red"]="#FF0000", ["Orange"]="#FF8600",
            ["Yellow"]="#FFFF00", ["Lime Green"]="#B1FF00", ["Green"]="#009E00",
            ["Powder Blue"]="#ACF0FF", ["Sky Blue"]="#00D6FA", ["Blue"]="#4550FF",
            ["Dark Blue"]="#0700F5", ["Purple"]="#BA00FF", ["Pink"]="#FF81E8",
            ["White"]="#FFFFFF", ["Grey"]="#959595", ["Black"]="#404040", ["Brown"]="#9E6E00"
        };

        var byGroup = _entries.GroupBy(e => e.Group)
            .ToDictionary(g => g.Key, g => g.ToList());

        _groups = groupOrder
            .Where(name => byGroup.ContainsKey(name))
            .Select(name => new ColorGroup(name, repHex.GetValueOrDefault(name, "#808080"), byGroup[name]))
            .ToList();
    }

    public IReadOnlyList<ColorGroup> Groups => _groups ?? [];
    public IReadOnlyList<ColorEntry> AllColors => _entries ?? [];

    public string? GetGroupForEnum(string enumName) =>
        _entries?.FirstOrDefault(e => e.EnumName == enumName)?.Group;

    public IEnumerable<ColorEntry> GetColorsForGroup(string groupName) =>
        _entries?.Where(e => e.Group == groupName) ?? [];
}

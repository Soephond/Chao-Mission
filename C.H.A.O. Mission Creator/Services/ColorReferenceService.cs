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

        _entries = new List<ColorEntry>();
        foreach (var el in doc.RootElement.GetProperty("colors").EnumerateArray())
        {
            var groupName = ColorGroupNameExtensions.FromDisplayName(el.GetProperty("group").GetString()!);
            _entries.Add(new ColorEntry(
                el.GetProperty("enum_name").GetString()!,
                el.GetProperty("value").GetInt32(),
                el.GetProperty("hex").GetString()!,
                groupName
            ));
        }

        _groups = Enum.GetValues<ColorGroupName>()
            .Select(g => new ColorGroup(g, _entries.Where(e => e.Group == g).ToList()))
            .Where(g => g.Colors.Count > 0)
            .ToList();
    }

    public IReadOnlyList<ColorGroup> Groups => _groups ?? [];
    public IReadOnlyList<ColorEntry> AllColors => _entries ?? [];

    public ColorGroupName? GetGroupForEnum(string enumName) =>
        _entries?.FirstOrDefault(e => e.EnumName == enumName)?.Group;

    public IEnumerable<ColorEntry> GetColorsForGroup(ColorGroupName group) =>
        _entries?.Where(e => e.Group == group) ?? [];
}
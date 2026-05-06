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
        _groups  = new List<ColorGroup>();

        // JSON structure: { "GroupName": { "hex": "#...", "colors": [ { "name": "...", "value": 0, "hex": "#..." } ] } }
        foreach (var groupProp in doc.RootElement.EnumerateObject())
        {
            ColorGroupName groupName;
            try { groupName = ColorGroupNameExtensions.FromDisplayName(groupProp.Name); }
            catch { continue; } // skip unknown groups

            var groupColors = new List<ColorEntry>();

            foreach (var colorEl in groupProp.Value.GetProperty("colors").EnumerateArray())
            {
                var colorName = colorEl.GetProperty("name").GetString()!;
                var enumName  = $"{JsonKeys.ColorPrefix}{colorName}";
                var value     = colorEl.GetProperty("value").GetInt32();
                var hex       = colorEl.GetProperty("hex").GetString()!;

                var entry = new ColorEntry(enumName, value, hex, groupName);
                groupColors.Add(entry);
                _entries.Add(entry);
            }

            _groups.Add(new ColorGroup(groupName, groupColors));
        }
    }

    public IReadOnlyList<ColorGroup> Groups    => _groups  ?? [];
    public IReadOnlyList<ColorEntry> AllColors => _entries ?? [];

    public ColorGroupName? GetGroupForEnum(string enumName) =>
        _entries?.FirstOrDefault(e => e.EnumName == enumName)?.Group;

    public IEnumerable<ColorEntry> GetColorsForGroup(ColorGroupName group) =>
        _entries?.Where(e => e.Group == group) ?? [];
}
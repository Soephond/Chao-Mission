using System.Text.Json;

namespace C.H.A.O._Mission_Creator.Services;

public class AppSettingsService
{
    private static readonly string SettingsPath = Path.Combine(
        Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData),
        "ChaoMissionCreator", "settings.json");

    public string? Sa2Path { get; set; }
    public bool EnableAllChecks { get; set; }
    public bool EnableAdvancedColorCheck { get; set; }

    public async Task LoadAsync()
    {
        if (!File.Exists(SettingsPath)) return;
        try
        {
            await using var stream = File.OpenRead(SettingsPath);
            var doc = await JsonDocument.ParseAsync(stream);
            var root = doc.RootElement;
            if (root.TryGetProperty("sa2Path", out var p)) Sa2Path = p.GetString();
            if (root.TryGetProperty("enableAllChecks", out var e)) EnableAllChecks = e.GetBoolean();
            if (root.TryGetProperty("enableAdvancedColorCheck", out var a)) EnableAdvancedColorCheck = a.GetBoolean();
        }
        catch { /* corrupt settings — ignore */ }
    }

    public async Task SaveAsync()
    {
        Directory.CreateDirectory(Path.GetDirectoryName(SettingsPath)!);
        await using var stream = File.Create(SettingsPath);
        await JsonSerializer.SerializeAsync(stream, new
        {
            sa2Path = Sa2Path,
            enableAllChecks = EnableAllChecks,
            enableAdvancedColorCheck = EnableAdvancedColorCheck,
        }, new JsonSerializerOptions { WriteIndented = true });
    }
}

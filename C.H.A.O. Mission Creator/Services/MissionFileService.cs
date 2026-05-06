using C.H.A.O._Mission_Creator.Models;
using Windows.Storage;
using Windows.Storage.Pickers;
using WinRT.Interop;

namespace C.H.A.O._Mission_Creator.Services;

public class MissionFileService
{
    private readonly MissionSerializer _serializer;

    public MissionFileService(MissionSerializer serializer) => _serializer = serializer;

    public MissionEditorModel LoadFromContent(string jsonContent) =>
        _serializer.Deserialize(jsonContent);

    public async Task<string?> SaveAsAsync(MissionEditorModel model)
    {
        var picker = new FileSavePicker
        {
            SuggestedStartLocation = PickerLocationId.DocumentsLibrary,
            SuggestedFileName = string.IsNullOrWhiteSpace(model.Name) ? "mission" : model.Name,
        };
        picker.FileTypeChoices.Add("Mission JSON", new List<string> { ".json" });

        InitializeWithWindow.Initialize(picker, GetAppHwnd());

        var file = await picker.PickSaveFileAsync();
        if (file is null) return null;

        var json = _serializer.Serialize(model);
        await FileIO.WriteTextAsync(file, json);
        return file.Path;
    }

    public async Task<string> SaveToBacklogAsync(MissionEditorModel model, string sa2Path)
    {
        var dir = Path.Combine(sa2Path, "mods", "C.H.A.O", "Missions", "Backlog");
        Directory.CreateDirectory(dir);

        var name = string.IsNullOrWhiteSpace(model.Name) ? "mission" : model.Name;
        var path = Path.Combine(dir, $"{name}.json");
        var json = _serializer.Serialize(model);
        await File.WriteAllTextAsync(path, json);
        return path;
    }

    private static IntPtr GetAppHwnd()
    {
        var window = Application.Current?.Windows[0];
        if (window?.Handler?.PlatformView is Microsoft.UI.Xaml.Window nativeWindow)
            return WindowNative.GetWindowHandle(nativeWindow);
        return IntPtr.Zero;
    }
}

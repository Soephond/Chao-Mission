using C.H.A.O._Mission_Creator.Services;
using Microsoft.Extensions.Logging;

namespace C.H.A.O._Mission_Creator;

public static class MauiProgram
{
    public static MauiApp CreateMauiApp()
    {
        var builder = MauiApp.CreateBuilder();
        builder
            .UseMauiApp<App>()
            .ConfigureFonts(fonts => { fonts.AddFont("OpenSans-Regular.ttf", "OpenSansRegular"); });

        builder.Services.AddMauiBlazorWebView();
        builder.Services.AddSingleton<ColorReferenceService>();
        builder.Services.AddSingleton<AppSettingsService>();
        builder.Services.AddSingleton<MissionSerializer>();
        builder.Services.AddSingleton<MissionFileService>();

#if DEBUG
        builder.Services.AddBlazorWebViewDeveloperTools();
        builder.Logging.AddDebug();
#endif

        return builder.Build();
    }
}
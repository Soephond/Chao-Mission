namespace C.H.A.O._Mission_Creator;

public partial class App : Application
{
    public App()
    {
        InitializeComponent();
    }

    protected override Window CreateWindow(IActivationState? activationState)
    {
        return new Window(new MainPage()) { Title = "C.H.A.O. Mission Creator" };
    }
}
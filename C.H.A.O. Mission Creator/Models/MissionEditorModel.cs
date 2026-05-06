namespace C.H.A.O._Mission_Creator.Models;

public class MissionEditorModel
{
    public string Name { get; set; } = "";
    public List<string> DescriptionLines { get; set; } = new();
    public List<RequirementModel> Requirements { get; set; } = new();
    public List<RewardModel> Rewards { get; set; } = new();
    public List<RequirementModel> BonusRequirements { get; set; } = new();
    public List<RewardModel> BonusRewards { get; set; } = new();
}
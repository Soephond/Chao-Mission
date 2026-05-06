using System.Text;
using System.Text.Json;
using C.H.A.O._Mission_Creator.Models;

namespace C.H.A.O._Mission_Creator.Services;

public class MissionSerializer
{
    private readonly ColorReferenceService _colors;

    public MissionSerializer(ColorReferenceService colors) => _colors = colors;

    // ── Deserialize ──────────────────────────────────────────────────────────

    public MissionEditorModel Deserialize(string json)
    {
        using var doc = JsonDocument.Parse(json);
        var root = doc.RootElement;
        var model = new MissionEditorModel();

        model.Name = root.GetProperty("Name").GetString() ?? "";

        foreach (var line in root.GetProperty("Description").EnumerateArray())
            model.DescriptionLines.Add(line.GetString() ?? "");

        model.Requirements = ReadRequirements(root.GetProperty("Requirements"));
        model.Rewards = ReadRewards(root.GetProperty("Rewards"));

        if (root.TryGetProperty("Bonus Requirements", out var bonusReqs))
            model.BonusRequirements = ReadRequirements(bonusReqs);

        if (root.TryGetProperty("Bonus Rewards", out var bonusRews))
            model.BonusRewards = ReadRewards(bonusRews);

        return model;
    }

    private List<RequirementModel> ReadRequirements(JsonElement el)
    {
        var list = new List<RequirementModel>();
        foreach (var req in el.EnumerateArray())
            list.Add(ReadRequirement(req));
        return list;
    }

    private RequirementModel ReadRequirement(JsonElement el)
    {
        var model = new RequirementModel
        {
            Type = Enum.Parse<RequirementType>(el.GetProperty("Type").GetString()!),
            Description = el.GetProperty("Description").GetString() ?? "",
            Checks = new(),
        };

        foreach (var branch in el.GetProperty("Checks").EnumerateArray())
        {
            var andList = new List<CheckModel>();
            foreach (var check in branch.EnumerateArray())
                andList.Add(ReadCheck(check));
            model.Checks.Add(andList);
        }

        TryCollapseColorGroup(model);
        return model;
    }

    private static CheckModel ReadCheck(JsonElement el)
    {
        var checkType = el.GetProperty("Type").GetString()!;
        var cm = new CheckModel { CheckType = checkType };

        bool hasValue = el.TryGetProperty("Value", out var valEl);
        bool hasMin   = el.TryGetProperty("MinValue", out var minEl);
        bool hasMax   = el.TryGetProperty("MaxValue", out var maxEl);

        cm.Value    = hasValue ? ReadValueString(valEl) : null;
        cm.MinValue = hasMin   ? ReadValueString(minEl) : null;
        cm.MaxValue = hasMax   ? ReadValueString(maxEl) : null;

        cm.RangeMode = (hasValue, hasMin, hasMax) switch
        {
            (true, _, _)           => RangeMode.Exact,
            (false, true, true)    => RangeMode.Range,
            (false, true, false)   => RangeMode.Min,
            (false, false, true)   => RangeMode.Max,
            _                      => RangeMode.Exact,
        };

        if (el.TryGetProperty("Inverted", out var inv)) cm.Inverted = inv.GetBoolean();
        if (el.TryGetProperty("Skill", out var skill))  cm.Skill = skill.GetString();
        if (el.TryGetProperty("Character", out var ch)) cm.Character = ch.GetString();

        return cm;
    }

    private static string ReadValueString(JsonElement el) =>
        el.ValueKind == JsonValueKind.String ? el.GetString()! : el.GetRawText();

    private void TryCollapseColorGroup(RequirementModel req)
    {
        if (req.Checks.Count == 0) return;
        if (!req.Checks.All(branch =>
            branch.Count == 1 &&
            branch[0].CheckType == nameof(ValueCheckType.ColorCheck) &&
            branch[0].Value is not null &&
            branch[0].RangeMode == RangeMode.Exact &&
            !branch[0].Inverted))
            return;

        var enumNames = req.Checks.Select(b => b[0].Value!).ToList();
        var groups = enumNames.Select(n => _colors.GetGroupForEnum(n)).ToList();

        if (groups.Any(g => g is null)) return;
        if (groups.Distinct().Count() != 1) return;

        var groupName = groups[0]!;
        req.Checks = new List<List<CheckModel>>
        {
            new() { new CheckModel { CheckType = nameof(ValueCheckType.ColorCheck), ColorGroup = groupName } }
        };
    }

    private static List<RewardModel> ReadRewards(JsonElement el)
    {
        var list = new List<RewardModel>();
        foreach (var rew in el.EnumerateArray())
            list.Add(ReadReward(rew));
        return list;
    }

    private static RewardModel ReadReward(JsonElement el)
    {
        var type = Enum.Parse<RewardType>(el.GetProperty("Type").GetString()!);
        var model = new RewardModel
        {
            Type = type,
            Description = el.GetProperty("Description").GetString() ?? "",
        };

        if (el.TryGetProperty("Amount", out var amt)) model.Amount = amt.GetInt32();

        if (type == RewardType.ChaoReward)
        {
            var v = el.GetProperty("Value");
            model.ChaoValue = new ChaoRewardValue
            {
                // eCHAO_TYPE is enum class — magic_enum returns plain member name e.g. "Child"
                ChaoType = v.TryGetProperty("ChaoType", out var ct) ? ReadValueString(ct) : "Child",
                Color    = v.TryGetProperty("Color",    out var co) ? ReadValueString(co) : "ChaoColor_Normal",
                Texture  = v.TryGetProperty("Texture",  out var tx) ? ReadValueString(tx) : "SA2BTexture_None",
                Tone     = v.TryGetProperty("Tone",     out var tn) ? ReadValueString(tn) : "ChaoTone_MonoTone",
                Shiny    = v.TryGetProperty("Shiny",    out var sh) ? ReadValueString(sh) : "ChaoShiny_None",
                Name     = v.TryGetProperty("Name",     out var nm) ? nm.GetString() ?? "" : "",
            };
        }
        else if (type != RewardType.RingReward && el.TryGetProperty("Value", out var enumVal))
        {
            model.EnumValue = ReadValueString(enumVal);
        }

        return model;
    }

    // ── Serialize ────────────────────────────────────────────────────────────

    public string Serialize(MissionEditorModel model)
    {
        var opts = new JsonWriterOptions { Indented = true };
        using var ms = new MemoryStream();
        using var w = new Utf8JsonWriter(ms, opts);

        w.WriteStartObject();
        w.WriteString("Name", model.Name);

        w.WriteStartArray("Description");
        foreach (var line in model.DescriptionLines) w.WriteStringValue(line);
        w.WriteEndArray();

        w.WriteStartArray("Requirements");
        foreach (var req in model.Requirements) WriteRequirement(w, req);
        w.WriteEndArray();

        w.WriteStartArray("Rewards");
        foreach (var rew in model.Rewards) WriteReward(w, rew);
        w.WriteEndArray();

        w.WriteStartArray("Bonus Requirements");
        foreach (var req in model.BonusRequirements) WriteRequirement(w, req);
        w.WriteEndArray();

        w.WriteStartArray("Bonus Rewards");
        foreach (var rew in model.BonusRewards) WriteReward(w, rew);
        w.WriteEndArray();

        w.WriteEndObject();
        w.Flush();
        return Encoding.UTF8.GetString(ms.ToArray());
    }

    private void WriteRequirement(Utf8JsonWriter w, RequirementModel req)
    {
        w.WriteStartObject();
        w.WriteString("Type", req.Type.ToString());
        w.WriteString("Description", req.Description);
        w.WriteStartArray("Checks");

        foreach (var branch in req.Checks)
        {
            // Expand color group into one OR branch per color in the group
            if (branch.Count == 1 && branch[0].ColorGroup is { } grp)
            {
                foreach (var entry in _colors.GetColorsForGroup(grp))
                {
                    w.WriteStartArray();
                    w.WriteStartObject();
                    w.WriteString("Type", nameof(ValueCheckType.ColorCheck));
                    w.WriteString("Value", entry.EnumName);
                    w.WriteEndObject();
                    w.WriteEndArray();
                }
            }
            else
            {
                w.WriteStartArray();
                foreach (var check in branch) WriteCheck(w, check);
                w.WriteEndArray();
            }
        }

        w.WriteEndArray();
        w.WriteEndObject();
    }

    private static void WriteCheck(Utf8JsonWriter w, CheckModel c)
    {
        w.WriteStartObject();
        w.WriteString("Type", c.CheckType);

        if (c.Skill is not null)     w.WriteString("Skill", c.Skill);
        if (c.Character is not null) w.WriteString("Character", c.Character);

        switch (c.RangeMode)
        {
            case RangeMode.Exact: WriteValue(w, "Value",    c.Value);    break;
            case RangeMode.Min:   WriteValue(w, "MinValue", c.MinValue); break;
            case RangeMode.Max:   WriteValue(w, "MaxValue", c.MaxValue); break;
            case RangeMode.Range:
                WriteValue(w, "MinValue", c.MinValue);
                WriteValue(w, "MaxValue", c.MaxValue);
                break;
        }

        if (c.Inverted) w.WriteBoolean("Inverted", true);
        w.WriteEndObject();
    }

    private static void WriteValue(Utf8JsonWriter w, string key, string? val)
    {
        if (val is null) return;
        if (int.TryParse(val, out var n))
            w.WriteNumber(key, n);
        else
            w.WriteString(key, val);
    }

    private static void WriteReward(Utf8JsonWriter w, RewardModel r)
    {
        w.WriteStartObject();
        w.WriteString("Type", r.Type.ToString());
        w.WriteNumber("Amount", r.Amount);

        if (r.Type == RewardType.ChaoReward && r.ChaoValue is { } cv)
        {
            w.WriteStartObject("Value");
            // ChaoType: plain member name for enum class (e.g. "Child")
            // or integer — write as-is (string or number)
            WriteValue(w, "ChaoType", cv.ChaoType);
            WriteValue(w, "Color",    cv.Color);
            WriteValue(w, "Texture",  cv.Texture);
            WriteValue(w, "Tone",     cv.Tone);
            WriteValue(w, "Shiny",    cv.Shiny);
            w.WriteString("Name", cv.Name);
            w.WriteEndObject();
        }
        else if (r.EnumValue is not null)
        {
            WriteValue(w, "Value", r.EnumValue);
        }

        w.WriteString("Description", r.Description);
        w.WriteEndObject();
    }
}
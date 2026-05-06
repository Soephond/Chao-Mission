using System.Text;
using System.Text.Json;
using C.H.A.O._Mission_Creator.Models;

namespace C.H.A.O._Mission_Creator.Services;

public class MissionSerializer
{
    private readonly ColorReferenceService _colors;

    public MissionSerializer(ColorReferenceService colors) => _colors = colors;

    // ── Enum ↔ JSON name helpers ─────────────────────────────────────────
    // ChaoSkill has no prefix in C# — add "ChaoSkill_" for JSON
    private static string    SkillToJson(ChaoSkill s)       => $"{JsonKeys.SkillPrefix}{s}";
    private static ChaoSkill SkillFromJson(string s)        => Enum.Parse<ChaoSkill>(s.Replace(JsonKeys.SkillPrefix, ""));

    // CharacterBondOrder already carries its prefix (CharacterBond_Sonic)
    private static string             CharacterToJson(CharacterBondOrder c) => c.ToString();
    private static CharacterBondOrder CharacterFromJson(string s)           => Enum.Parse<CharacterBondOrder>(s);

    // ChaoReward typed field helpers
    // EChaoType is enum class — magic_enum returns plain member name ("Child"), no scope prefix
    private static string     EChaoTypeToJson(EChaoType t)    => t.ToString();
    private static EChaoType  EChaoTypeFromJson(JsonElement e) =>
        e.ValueKind == JsonValueKind.Number
            ? (EChaoType)e.GetInt32()
            : Enum.Parse<EChaoType>(e.GetString()!);

    private static string      TextureToJson(SA2BTexture t)    => $"{JsonKeys.TexturePrefix}{t}";
    private static SA2BTexture TextureFromJson(JsonElement e)  =>
        e.ValueKind == JsonValueKind.Number
            ? (SA2BTexture)e.GetInt32()
            : Enum.Parse<SA2BTexture>(e.GetString()!.Replace(JsonKeys.TexturePrefix, ""));

    private static string   ToneToJson(ChaoTone t)       => $"{JsonKeys.TonePrefix}{t}";
    private static ChaoTone ToneFromJson(JsonElement e)  =>
        e.ValueKind == JsonValueKind.Number
            ? (ChaoTone)e.GetInt32()
            : Enum.Parse<ChaoTone>(e.GetString()!.Replace(JsonKeys.TonePrefix, ""));

    private static string    ShinyToJson(ChaoShiny s)      => $"{JsonKeys.ShinyPrefix}{s}";
    private static ChaoShiny ShinyFromJson(JsonElement e)  =>
        e.ValueKind == JsonValueKind.Number
            ? (ChaoShiny)e.GetInt32()
            : Enum.Parse<ChaoShiny>(e.GetString()!.Replace(JsonKeys.ShinyPrefix, ""));

    // ── Deserialize ──────────────────────────────────────────────────────

    public MissionEditorModel Deserialize(string json)
    {
        using var doc = JsonDocument.Parse(json);
        var root = doc.RootElement;
        var model = new MissionEditorModel
        {
            Name = root.GetProperty(JsonKeys.MissionName).GetString() ?? "",
        };

        foreach (var line in root.GetProperty(JsonKeys.Description).EnumerateArray())
            model.DescriptionLines.Add(line.GetString() ?? "");

        model.Requirements = ReadRequirements(root.GetProperty(JsonKeys.Requirements));
        model.Rewards      = ReadRewards(root.GetProperty(JsonKeys.Rewards));

        if (root.TryGetProperty(JsonKeys.BonusRequirements, out var bonusReqs))
            model.BonusRequirements = ReadRequirements(bonusReqs);

        if (root.TryGetProperty(JsonKeys.BonusRewards, out var bonusRews))
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
            Type        = Enum.Parse<RequirementType>(el.GetProperty(JsonKeys.Type).GetString()!),
            Description = el.GetProperty(JsonKeys.Description).GetString() ?? "",
            Checks      = new(),
        };

        foreach (var branch in el.GetProperty(JsonKeys.Checks).EnumerateArray())
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
        var cm = new CheckModel
        {
            CheckType = Enum.Parse<ValueCheckType>(el.GetProperty(JsonKeys.Type).GetString()!),
        };

        bool hasValue = el.TryGetProperty(JsonKeys.Value,    out var valEl);
        bool hasMin   = el.TryGetProperty(JsonKeys.MinValue, out var minEl);
        bool hasMax   = el.TryGetProperty(JsonKeys.MaxValue, out var maxEl);

        cm.Value    = hasValue ? ReadValueString(valEl) : null;
        cm.MinValue = hasMin   ? ReadValueString(minEl) : null;
        cm.MaxValue = hasMax   ? ReadValueString(maxEl) : null;

        cm.RangeMode = (hasValue, hasMin, hasMax) switch
        {
            (true, _, _)         => RangeMode.Exact,
            (false, true, true)  => RangeMode.Range,
            (false, true, false) => RangeMode.Min,
            (false, false, true) => RangeMode.Max,
            _                    => RangeMode.Exact,
        };

        if (el.TryGetProperty(JsonKeys.Inverted,  out var inv) && inv.ValueKind == JsonValueKind.True)
            cm.Inverted = true;

        if (el.TryGetProperty(JsonKeys.Skill,     out var skill) && skill.GetString() is { } sk)
            cm.Skill = SkillFromJson(sk);

        if (el.TryGetProperty(JsonKeys.Character, out var ch)    && ch.GetString() is { } c)
            cm.Character = CharacterFromJson(c);

        return cm;
    }

    private static string ReadValueString(JsonElement el) =>
        el.ValueKind == JsonValueKind.String ? el.GetString()! : el.GetRawText();

    private void TryCollapseColorGroup(RequirementModel req)
    {
        if (req.Checks.Count == 0) return;
        if (!req.Checks.All(branch =>
            branch.Count == 1 &&
            branch[0].CheckType == ValueCheckType.ColorCheck &&
            branch[0].Value is not null &&
            branch[0].RangeMode == RangeMode.Exact &&
            !branch[0].Inverted))
            return;

        var groups = req.Checks
            .Select(b => _colors.GetGroupForEnum(b[0].Value!))
            .ToList();

        if (groups.Any(g => g is null)) return;
        if (groups.Select(g => g!.Value).Distinct().Count() != 1) return;

        req.Checks = new List<List<CheckModel>>
        {
            new() { new CheckModel { CheckType = ValueCheckType.ColorCheck, ColorGroup = groups[0]!.Value } }
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
        var type = Enum.Parse<RewardType>(el.GetProperty(JsonKeys.Type).GetString()!);
        var model = new RewardModel
        {
            Type        = type,
            Description = el.GetProperty(JsonKeys.Description).GetString() ?? "",
        };

        if (el.TryGetProperty(JsonKeys.Amount, out var amt))
            model.Amount = amt.GetInt32();

        if (type == RewardType.ChaoReward)
        {
            var v = el.GetProperty(JsonKeys.Value);
            model.ChaoValue = new ChaoRewardValue
            {
                ChaoType = v.TryGetProperty(JsonKeys.ChaoType, out var ct) ? EChaoTypeFromJson(ct)  : ModelDefaults.ChaoType,
                Color    = v.TryGetProperty(JsonKeys.Color,    out var co) ? ReadValueString(co)     : ModelDefaults.NormalColorEnumName,
                Texture  = v.TryGetProperty(JsonKeys.Texture,  out var tx) ? TextureFromJson(tx)     : ModelDefaults.Texture,
                Tone     = v.TryGetProperty(JsonKeys.Tone,     out var tn) ? ToneFromJson(tn)        : ModelDefaults.Tone,
                Shiny    = v.TryGetProperty(JsonKeys.Shiny,    out var sh) ? ShinyFromJson(sh)       : ModelDefaults.Shiny,
                Name     = v.TryGetProperty(JsonKeys.ChaoName, out var nm) ? nm.GetString() ?? ""   : "",
            };
        }
        else if (type != RewardType.RingReward && el.TryGetProperty(JsonKeys.Value, out var enumVal))
        {
            model.EnumValue = ReadValueString(enumVal);
        }

        return model;
    }

    // ── Serialize ────────────────────────────────────────────────────────

    public string Serialize(MissionEditorModel model)
    {
        using var ms = new MemoryStream();
        using var w  = new Utf8JsonWriter(ms, new JsonWriterOptions { Indented = true });

        w.WriteStartObject();
        w.WriteString(JsonKeys.MissionName, model.Name);

        w.WriteStartArray(JsonKeys.Description);
        foreach (var line in model.DescriptionLines) w.WriteStringValue(line);
        w.WriteEndArray();

        w.WriteStartArray(JsonKeys.Requirements);
        foreach (var req in model.Requirements) WriteRequirement(w, req);
        w.WriteEndArray();

        w.WriteStartArray(JsonKeys.Rewards);
        foreach (var rew in model.Rewards) WriteReward(w, rew);
        w.WriteEndArray();

        w.WriteStartArray(JsonKeys.BonusRequirements);
        foreach (var req in model.BonusRequirements) WriteRequirement(w, req);
        w.WriteEndArray();

        w.WriteStartArray(JsonKeys.BonusRewards);
        foreach (var rew in model.BonusRewards) WriteReward(w, rew);
        w.WriteEndArray();

        w.WriteEndObject();
        w.Flush();
        return Encoding.UTF8.GetString(ms.ToArray());
    }

    private void WriteRequirement(Utf8JsonWriter w, RequirementModel req)
    {
        w.WriteStartObject();
        w.WriteString(JsonKeys.Type,        req.Type.ToString());
        w.WriteString(JsonKeys.Description, req.Description);
        w.WriteStartArray(JsonKeys.Checks);

        foreach (var branch in req.Checks)
        {
            if (branch.Count == 1 && branch[0].ColorGroup is { } grp)
            {
                // Expand color group: one OR branch per color in the group
                foreach (var entry in _colors.GetColorsForGroup(grp))
                {
                    w.WriteStartArray();
                    w.WriteStartObject();
                    w.WriteString(JsonKeys.Type,  ValueCheckType.ColorCheck.ToString());
                    w.WriteString(JsonKeys.Value, entry.EnumName);
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
        w.WriteString(JsonKeys.Type, c.CheckType.ToString());

        if (c.Skill.HasValue)     w.WriteString(JsonKeys.Skill,     SkillToJson(c.Skill.Value));
        if (c.Character.HasValue) w.WriteString(JsonKeys.Character, CharacterToJson(c.Character.Value));

        switch (c.RangeMode)
        {
            case RangeMode.Exact: WriteValue(w, JsonKeys.Value,    c.Value);    break;
            case RangeMode.Min:   WriteValue(w, JsonKeys.MinValue, c.MinValue); break;
            case RangeMode.Max:   WriteValue(w, JsonKeys.MaxValue, c.MaxValue); break;
            case RangeMode.Range:
                WriteValue(w, JsonKeys.MinValue, c.MinValue);
                WriteValue(w, JsonKeys.MaxValue, c.MaxValue);
                break;
        }

        if (c.Inverted) w.WriteBoolean(JsonKeys.Inverted, true);
        w.WriteEndObject();
    }

    private static void WriteValue(Utf8JsonWriter w, string key, string? val)
    {
        if (val is null) return;
        if (int.TryParse(val, out var n)) w.WriteNumber(key, n);
        else                              w.WriteString(key, val);
    }

    private static void WriteReward(Utf8JsonWriter w, RewardModel r)
    {
        w.WriteStartObject();
        w.WriteString(JsonKeys.Type,   r.Type.ToString());
        w.WriteNumber(JsonKeys.Amount, r.Amount);

        if (r.Type == RewardType.ChaoReward && r.ChaoValue is { } cv)
        {
            w.WriteStartObject(JsonKeys.Value);
            w.WriteString(JsonKeys.ChaoType, EChaoTypeToJson(cv.ChaoType));
            WriteValue(w,  JsonKeys.Color,   cv.Color);           // string (no C# enum for ChaoColor)
            w.WriteString(JsonKeys.Texture,  TextureToJson(cv.Texture));
            w.WriteString(JsonKeys.Tone,     ToneToJson(cv.Tone));
            w.WriteString(JsonKeys.Shiny,    ShinyToJson(cv.Shiny));
            w.WriteString(JsonKeys.ChaoName, cv.Name);
            w.WriteEndObject();
        }
        else if (r.EnumValue is not null)
        {
            WriteValue(w, JsonKeys.Value, r.EnumValue);
        }

        w.WriteString(JsonKeys.Description, r.Description);
        w.WriteEndObject();
    }
}

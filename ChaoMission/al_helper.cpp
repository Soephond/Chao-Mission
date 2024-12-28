#include "pch.h"
#include "al_helper.h"

#include <iostream>
#include <ostream>

wchar_t ChaoNameDecoder[256]
{
    u'\n',
    u'!',
    u'\"',
    u'#',
    u'$',
    u'%',
    u'&',
    u'\'',
    u'(',
    u')',
    u'*',
    u'+',
    u',',
    u'-',
    u'.',
    u'/',
    u'0',
    u'1',
    u'2',
    u'3',
    u'4',
    u'5',
    u'6',
    u'7',
    u'8',
    u'9',
    u':',
    u';',
    u'<',
    u'=',
    u'>',
    u'?',
    u'@',
    u'A',
    u'B',
    u'C',
    u'D',
    u'E',
    u'F',
    u'G',
    u'H',
    u'I',
    u'J',
    u'K',
    u'L',
    u'M',
    u'N',
    u'O',
    u'P',
    u'Q',
    u'R',
    u'S',
    u'T',
    u'U',
    u'V',
    u'W',
    u'X',
    u'Y',
    u'Z',
    u'[',
    u'¥',
    u']',
    u'^',
    u'_',
    u'‘',
    u'a',
    u'b',
    u'c',
    u'd',
    u'e',
    u'f',
    u'g',
    u'h',
    u'i',
    u'j',
    u'k',
    u'l',
    u'm',
    u'n',
    u'o',
    u'p',
    u'q',
    u'r',
    u's',
    u't',
    u'u',
    u'v',
    u'w',
    u'x',
    u'y',
    u'z',
    u'{',
    u'|',
    u'}',
    u'~',
    u' ',
    u'À',
    u'Á',
    u'Â',
    u'Ã',
    u'Ä',
    u'Å',
    u'Æ',
    u'Ç',
    u'È',
    u'É',
    u'Ê',
    u'Ë',
    u'Ì',
    u'Í',
    u'Î',
    u'Ï',
    u'Ð',
    u'Ñ',
    u'Ò',
    u'Ó',
    u'Ô',
    u'Õ',
    u'Ö',
    u'¿',
    u'Ø',
    u'Ù',
    u'Ú',
    u'Û',
    u'Ü',
    u'Ý',
    u'Þ',
    u'ß',
    u'à',
    u'á',
    u'â',
    u'ã',
    u'ä',
    u'å',
    u'æ',
    u'ç',
    u'è',
    u'é',
    u'ê',
    u'ë',
    u'ì',
    u'í',
    u'î',
    u'ï',
    u'ð',
    u'ñ',
    u'ò',
    u'ó',
    u'ô',
    u'õ',
    u'ö',
    u'¡',
    u'ø',
    u'ù',
    u'ú',
    u'û',
    u'ü',
    u'ý',
    u'þ',
    u'ÿ',
    u'ァ',
    u'ア',
    u'ィ',
    u'イ',
    u'ゥ',
    u'ウ',
    u'ェ',
    u'エ',
    u'ォ',
    u'オ',
    u'カ',
    u'ガ',
    u'キ',
    u'ギ',
    u'ク',
    u'グ',
    u'ケ',
    u'ゲ',
    u'コ',
    u'ゴ',
    u'サ',
    u'ザ',
    u'シ',
    u'ジ',
    u'ス',
    u'ズ',
    u'セ',
    u'ゼ',
    u'ソ',
    u'ゾ',
    u'タ',
    u'ダ',
    u'チ',
    u'ヂ',
    u'ツ',
    u'ッ',
    u'ヅ',
    u'テ',
    u'デ',
    u'ト',
    u'ド',
    u'ナ',
    u'ニ',
    u'ヌ',
    u'ネ',
    u'ノ',
    u'ハ',
    u'バ',
    u'パ',
    u'ヒ',
    u'ビ',
    u'ピ',
    u'フ',
    u'ブ',
    u'プ',
    u'ヘ',
    u'ベ',
    u'ベ',
    u'ホ',
    u'ボ',
    u'ポ',
    u'マ',
    u'ミ',
    u'ム',
    u'メ',
    u'モ',
    u'ャ',
    u'ヤ',
    u'ュ',
    u'ユ',
    u'ョ',
    u'ヨ',
    u'ラ',
    u'リ',
    u'ル',
    u'レ',
    u'ロ',
    u'ヮ',
    u'ワ',
    u'ﾞ',
    u'ﾟ',
    u'ヲ',
    u'ン',
    u'。',
    u'、',
    u'〒',
    u'・',
    u'★',
    u'♀',
    u'♂',
    u'♪',
    u'…',
    u'「',
    u'」',
    u'ヴ',
    u' '
};

char getPositionAsChar(int target) {
    for (int i = 0; i < 256; ++i) {
        if (ChaoNameDecoder[i] == target) {
            return (char)i; // Return the index if the target is found
        }
    }
    return -1; // Return -1 if the target is not found
}

bool DecodeChaoName(wchar_t Name[], wchar_t* ChaoNameDecoded, int size)
{
    int i = 0;
    wchar_t* result = ChaoNameDecoded;
    while (Name[i] != 0 && i < size)
    {
        if (Name[i] < 256) result[i] = ChaoNameDecoder[Name[i]];
        else result[i] = '_';
        i++;
    }
    return true;
}

bool EncodeChaoName(wchar_t Name[], wchar_t* ChaoNameEncoded, int size)
{
    int i = 0;
    wchar_t* result = ChaoNameEncoded;
    while (Name[i] != 0 && i < size)
    {
        if (Name[i] < 256) result[i] = getPositionAsChar(Name[i]);
        else result[i] = '_';
        i++;
    }
    return true;
}

bool EncodeChaoNameFromString(const char* Name, char* ChaoNameEncoded, int size)
{
    int i = 0;
    char* result = ChaoNameEncoded;
    while (Name[i] != 0 && i < size)
    {
        if (Name[i] < 256) result[i] = getPositionAsChar(Name[i]);
        else result[i] = '_';
        std::cout << result[i] << std::endl;
        i++;
    }
    return true;
}

void RingDisplay_AddRings(signed int a1)
{
    __asm
        {
        mov eax, [a1]
        call sub_5A6B30
        }
}

void AL_GeneAnalyzeCommon(ChaoDNA* a1, ChaoDataBase* a2)
{
    __asm
    {
        push [a2]
        mov edi, [a1]
        call sub_551DA0
    }
}

NJS_POINT3 (*GetSetPosGarden(eCHAO_STAGE_NUMBER stage))[16]
{
    switch (stage)
    {
    case CHAO_STG_NEUT:
        return (NJS_POINT3(*)[16])&ChaoSetPositionNeut;

    case CHAO_STG_HERO:
        return (NJS_POINT3(*)[16])&ChaoSetPositionHero;

    case CHAO_STG_DARK:
        return (NJS_POINT3(*)[16])&ChaoSetPositionDark;

    default:
        return nullptr;
    }
}

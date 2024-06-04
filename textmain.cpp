#include <iostream>
#include <string>
#include <fstream>
#include <codecvt>
#include<Windows.h>
#include<iomanip>
#include <vector>


std::locale& GetCVT_UTF8()
{
    static auto cvtUTF8 = std::locale
    (
        std::locale::empty(),
        new std::codecvt_utf8
        <
        wchar_t, 0x10ffff,
        std::codecvt_mode(std::consume_header | std::generate_header | std::little_endian)
        >

    );
    return cvtUTF8;
}

void SteTOWStr(std::string& strString, std::wstring& wsString, unsigned int uCodePage)
{
    int len = 0;
    wchar_t* buffer = nullptr;

    len = MultiByteToWideChar(uCodePage, NULL, strString.c_str(), static_cast<int>(strString.size()), NULL, NULL);
    buffer = new wchar_t[len + 1];
    MultiByteToWideChar(uCodePage, NULL, strString.c_str(), static_cast<int>(strString.size()), buffer, len);
    buffer[len] = L'\0';
    wsString = buffer;
}

void dump(std::string strScript)
{
    std::wofstream woText(strScript + ".txt");
    woText.imbue(GetCVT_UTF8());
    std::ifstream iScript(strScript);
    if (!(iScript.is_open() && woText.is_open()))
    {
        std::cout << "Error opening file." << std::endl;
        return;
    }
    size_t count = 1;
    std::wstring wLine;
    for (std::string Line; std::getline(iScript, Line); count++)
    {
        if (((unsigned char)Line[0] > 0x7F && (unsigned char)Line[1] != 0x93) || (Line.find("//") == 0))
        {
            if ((unsigned char)Line[0] == 0x81 && (unsigned char)Line[1] == 0x40)
            {
                SteTOWStr(Line, wLine, 932);
                woText << '\n' << L"cots:" << std::setw(0x8) << std::setfill(L'0') << count;
                woText << '\n' << wLine.substr(1) << '\n';
            }
            else
            {
                woText << '\n' << L"cot:" << std::setw(0x8) << std::setfill(L'0') << count;
                SteTOWStr(Line, wLine, 932);
                woText << '\n' << wLine << '\n';
            }
        }
    }

    woText.flush();
    woText.close();
    iScript.close();
}

int main()
{
    system("chcp 932");
    dump("0001.txt");

    return 0;
}

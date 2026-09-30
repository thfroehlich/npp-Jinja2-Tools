#include "Jinja2Tools/Utf8.h"
#include "Jinja2Tools/Win32Error.h"

#include <windows.h>

namespace jinja2tools
{
    std::string toUtf8(std::wstring_view value)
    {
        if (value.empty())
        {
            return {};
        }

        const int size =
            WideCharToMultiByte(
                CP_UTF8,
                WC_ERR_INVALID_CHARS,
                value.data(),
                static_cast<int>(value.size()),
                nullptr,
                0,
                nullptr,
                nullptr);

        if (size == 0)
        {
            throw makeWin32Error(
                "WideCharToMultiByte",
                GetLastError());
        }

        std::string result(size, '\0');

        WideCharToMultiByte(
            CP_UTF8,
            WC_ERR_INVALID_CHARS,
            value.data(),
            static_cast<int>(value.size()),
            result.data(),
            size,
            nullptr,
            nullptr);

        return result;
    }

    std::wstring fromUtf8(std::string_view value)
    {
        if (value.empty())
        {
            return {};
        }

        const int size =
            MultiByteToWideChar(
                CP_UTF8,
                MB_ERR_INVALID_CHARS,
                value.data(),
                static_cast<int>(value.size()),
                nullptr,
                0);

        if (size == 0)
        {
            throw makeWin32Error(
                "MultiByteToWideChar",
                GetLastError());
        }

        std::wstring result(size, L'\0');

        MultiByteToWideChar(
            CP_UTF8,
            MB_ERR_INVALID_CHARS,
            value.data(),
            static_cast<int>(value.size()),
            result.data(),
            size);

        return result;
    }
}
#include "Jinja2Tools/Win32Error.h"
#include "Jinja2Tools/Utf8.h"

#include <algorithm>

namespace jinja2tools
{
    std::wstring formatWin32Error(
        DWORD errorCode)
    {
        wchar_t* buffer = nullptr;

        const DWORD size =
            FormatMessageW(
                FORMAT_MESSAGE_ALLOCATE_BUFFER |
                FORMAT_MESSAGE_FROM_SYSTEM |
                FORMAT_MESSAGE_IGNORE_INSERTS,
                nullptr,
                errorCode,
                0,
                reinterpret_cast<wchar_t*>(&buffer),
                0,
                nullptr);

        std::wstring message =
            size
                ? std::wstring(buffer, size)
                : L"Unknown Win32 error";

        if (buffer)
        {
            LocalFree(buffer);
        }

        while (
            !message.empty() &&
            (
                message.back() == L'\r' ||
                message.back() == L'\n'
            ))
        {
            message.pop_back();
        }

        return message;
    }

    std::runtime_error makeWin32Error(
        std::string_view operation,
        DWORD errorCode)
    {
        return std::runtime_error(
            std::string(operation) +
            " failed. Win32 error " +
            std::to_string(errorCode) +
            ": " +
            toUtf8(formatWin32Error(errorCode)));
    }
}
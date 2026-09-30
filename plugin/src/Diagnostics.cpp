#include "Jinja2Tools/Diagnostics.h"
#include "Jinja2Tools/Utf8.h"

namespace jinja2tools
{
    Diagnostics::Diagnostics(
        const NppData& data)
        : nppData_(data)
    {
    }

    void Diagnostics::clear()
    {
    }

    void Diagnostics::show(
        const std::vector<Diagnostic>& diagnostics)
    {
        std::wstring message;

        for (const auto& diagnostic : diagnostics)
        {
            message +=
                fromUtf8(
                    diagnostic.code +
                    ": " +
                    diagnostic.message);

            message += L"\n";
        }

        MessageBoxW(
            nppData_._nppHandle,
            message.c_str(),
            L"Jinja2 Tools",
            MB_OK |
            (diagnostics.empty()
                ? MB_ICONINFORMATION
                : MB_ICONWARNING));
    }

    void Diagnostics::showErrorMessage(
        std::wstring_view title,
        std::wstring_view message)
    {
        MessageBoxW(
            nppData_._nppHandle,
            std::wstring(message).c_str(),
            std::wstring(title).c_str(),
            MB_OK | MB_ICONERROR);
    }

    void Diagnostics::showInformationMessage(
        std::wstring_view title,
        std::wstring_view message)
    {
        MessageBoxW(
            nppData_._nppHandle,
            std::wstring(message).c_str(),
            std::wstring(title).c_str(),
            MB_OK | MB_ICONINFORMATION);
    }
}
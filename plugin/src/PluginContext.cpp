#include "Jinja2Tools/PluginContext.h"
#include "Jinja2Tools/Win32Error.h"

#include <vector>

namespace jinja2tools
{
    PluginContext& PluginContext::instance()
    {
        static PluginContext context;

        return context;
    }

    void PluginContext::initialize(
        HMODULE moduleHandle,
        const NppData& nppData)
    {
        moduleHandle_ = moduleHandle;
        nppData_ = nppData;
    }

    HMODULE PluginContext::moduleHandle() const noexcept
    {
        return moduleHandle_;
    }

    const NppData& PluginContext::nppData() const noexcept
    {
        return nppData_;
    }

    std::filesystem::path PluginContext::pluginDirectory() const
    {
        std::vector<wchar_t> buffer(1024);

        for (;;)
        {
            DWORD length =
                GetModuleFileNameW(
                    moduleHandle_,
                    buffer.data(),
                    static_cast<DWORD>(buffer.size()));

            if (!length)
            {
                throw makeWin32Error(
                    "GetModuleFileNameW",
                    GetLastError());
            }

            if (length < buffer.size() - 1)
            {
                return std::filesystem::path(
                           std::wstring(
                               buffer.data(),
                               length))
                    .parent_path();
            }

            buffer.resize(buffer.size() * 2);
        }
    }

    std::filesystem::path PluginContext::helperExecutable() const
    {
        auto path =
            pluginDirectory() /
            L"Jinja2Tools.Helper.exe";

        return path;
    }
}
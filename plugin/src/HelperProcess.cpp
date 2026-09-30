#include "Jinja2Tools/HelperProcess.h"
#include "Jinja2Tools/Win32Error.h"

#include <thread>
#include <vector>

namespace
{
    struct Handle
    {
        HANDLE handle{};

        Handle() = default;

        explicit Handle(HANDLE value)
            : handle(value)
        {
        }

        ~Handle()
        {
            if (
                handle &&
                handle != INVALID_HANDLE_VALUE)
            {
                CloseHandle(handle);
            }
        }

        Handle(const Handle&) = delete;
        Handle& operator=(const Handle&) = delete;
    };

    void readAll(
        HANDLE handle,
        std::string& output)
    {
        char buffer[4096];

        DWORD bytesRead = 0;

        while (
            ReadFile(
                handle,
                buffer,
                sizeof(buffer),
                &bytesRead,
                nullptr) &&
            bytesRead > 0)
        {
            output.append(
                buffer,
                bytesRead);
        }
    }
}

namespace jinja2tools
{
    ProcessResult HelperProcess::execute(
        const std::filesystem::path& executable,
        std::string_view input,
        std::chrono::milliseconds timeout) const
    {
        SECURITY_ATTRIBUTES sa{};
        sa.nLength = sizeof(sa);
        sa.bInheritHandle = TRUE;

        HANDLE stdinReadRaw{};
        HANDLE stdinWriteRaw{};

        HANDLE stdoutReadRaw{};
        HANDLE stdoutWriteRaw{};

        HANDLE stderrReadRaw{};
        HANDLE stderrWriteRaw{};

        if (
            !CreatePipe(
                &stdinReadRaw,
                &stdinWriteRaw,
                &sa,
                0) ||

            !CreatePipe(
                &stdoutReadRaw,
                &stdoutWriteRaw,
                &sa,
                0) ||

            !CreatePipe(
                &stderrReadRaw,
                &stderrWriteRaw,
                &sa,
                0))
        {
            throw makeWin32Error(
                "CreatePipe",
                GetLastError());
        }

        Handle stdinRead(stdinReadRaw);
        Handle stdinWrite(stdinWriteRaw);

        Handle stdoutRead(stdoutReadRaw);
        Handle stdoutWrite(stdoutWriteRaw);

        Handle stderrRead(stderrReadRaw);
        Handle stderrWrite(stderrWriteRaw);

        SetHandleInformation(
            stdinWrite.handle,
            HANDLE_FLAG_INHERIT,
            0);

        SetHandleInformation(
            stdoutRead.handle,
            HANDLE_FLAG_INHERIT,
            0);

        SetHandleInformation(
            stderrRead.handle,
            HANDLE_FLAG_INHERIT,
            0);

        STARTUPINFOW startupInfo{};
        startupInfo.cb = sizeof(startupInfo);
        startupInfo.dwFlags =
            STARTF_USESTDHANDLES;

        startupInfo.hStdInput =
            stdinRead.handle;

        startupInfo.hStdOutput =
            stdoutWrite.handle;

        startupInfo.hStdError =
            stderrWrite.handle;

        PROCESS_INFORMATION processInfo{};

        //
        // KORREKTE Commandline
        //
        std::wstring commandLine =
            L"\"" +
            executable.wstring() +
            L"\"";

        std::vector<wchar_t> mutableCommand(
            commandLine.begin(),
            commandLine.end());

        mutableCommand.push_back(L'\0');

        // MessageBoxW(
        //    nullptr,
        //    commandLine.c_str(),
        //    L"Command Line",
        //    MB_OK);

        if (
            !CreateProcessW(
                nullptr,
                mutableCommand.data(),
                nullptr,
                nullptr,
                TRUE,
                CREATE_NO_WINDOW,
                nullptr,
                executable.parent_path().c_str(),
                &startupInfo,
                &processInfo))
        {
            throw makeWin32Error(
                "CreateProcessW",
                GetLastError());
        }

        // MessageBoxW(
        //    nullptr,
        //    L"CreateProcessW succeeded",
        //    L"Debug",
        //    MB_OK);

        Handle process(processInfo.hProcess);
        Handle thread(processInfo.hThread);

        CloseHandle(stdoutWrite.handle);
        stdoutWrite.handle = nullptr;

        CloseHandle(stderrWrite.handle);
        stderrWrite.handle = nullptr;

        CloseHandle(stdinRead.handle);
        stdinRead.handle = nullptr;

        ProcessResult result;

        std::thread stdoutThread(
            readAll,
            stdoutRead.handle,
            std::ref(
                result.standardOutput));

        std::thread stderrThread(
            readAll,
            stderrRead.handle,
            std::ref(
                result.standardError));

        DWORD written = 0;
        size_t totalWritten = 0;

        while (totalWritten < input.size())
        {
            if (
                !WriteFile(
                    stdinWrite.handle,
                    input.data() +
                        totalWritten,
                    static_cast<DWORD>(
                        input.size() -
                        totalWritten),
                    &written,
                    nullptr))
            {
                break;
            }

            totalWritten += written;
        }

        CloseHandle(stdinWrite.handle);
        stdinWrite.handle = nullptr;

        // MessageBoxW(
        //    nullptr,
        //    L"Waiting for helper...",
        //    L"Debug",
        //    MB_OK);

        DWORD waitResult =
            WaitForSingleObject(
                process.handle,
                static_cast<DWORD>(
                    timeout.count()));

        // MessageBoxW(
        //    nullptr,
        //    L"Helper finished",
        //    L"Debug",
        //    MB_OK);

        result.timedOut =
            waitResult ==
            WAIT_TIMEOUT;

        if (result.timedOut)
        {
            TerminateProcess(
                process.handle,
                124);
        }

        WaitForSingleObject(
            process.handle,
            INFINITE);

        GetExitCodeProcess(
            process.handle,
            &result.exitCode);

        // MessageBoxW(
        //    nullptr,
        //    L"Joining stdout thread",
        //    L"Debug",
        //    MB_OK);

        stdoutThread.join();
        stderrThread.join();

        // MessageBoxW(
        //    nullptr,
        //    L"Joining stderr thread",
        //    L"Debug",
        //    MB_OK);

        return result;
    }
}
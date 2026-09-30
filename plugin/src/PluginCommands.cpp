#include "Jinja2Tools/PluginCommands.h"

#include "Jinja2Tools/Diagnostics.h"
#include "Jinja2Tools/EditorService.h"
#include "Jinja2Tools/HelperClient.h"
#include "Jinja2Tools/PluginContext.h"
#include "Jinja2Tools/Utf8.h"
#include "Jinja2Tools/Version.h"

#include <NotepadPlusPlus/Notepad_plus_msgs.h>

#include <array>
#include <chrono>
#include <exception>
#include <string>

namespace
{
    void setCurrentLanguageToHtml()
    {
        const auto& nppData =
            jinja2tools::PluginContext::instance().nppData();

        SendMessageW(
            nppData._nppHandle,
            NPPM_SETCURRENTLANGTYPE,
            0,
            static_cast<LPARAM>(L_HTML));
    }
}

namespace jinja2tools
{
    namespace
    {
        std::array<FuncItem, 6> commandItems{};

        void setCommand(
            int index,
            const wchar_t* name,
            PFUNCPLUGINCMD function)
        {
            wcscpy_s(
                commandItems[index]._itemName,
                name);

            commandItems[index]._pFunc =
                function;
        }

        HelperResponse runHelper(
            Operation operation,
            const std::string& documentText)
        {
            auto& context =
                PluginContext::instance();

            HelperClient client(
                context.helperExecutable(),
                std::chrono::seconds(10));

            return client.execute(
                operation,
                {
                    "active.j2",
                    documentText
                });
        }

        void showException(
            const std::exception& exception)
        {
            auto& context =
                PluginContext::instance();

            Diagnostics diagnostics(
                context.nppData());

            diagnostics.showErrorMessage(
                L"Jinja2 Tools",
                fromUtf8(exception.what()));
        }
    }

    void initializeCommands()
    {
        setCommand(
            0,
            L"Format Document",
            commandFormatDocument);

        setCommand(
            1,
            L"Format Selection",
            commandFormatSelection);

        setCommand(
            2,
            L"Check Syntax",
            commandCheckSyntax);

        setCommand(
            3,
            L"Analyze Template",
            commandAnalyzeTemplate);

        setCommand(
            4,
            L"Clear Diagnostics",
            commandClearDiagnostics);

        setCommand(
            5,
            L"About",
            commandAbout);
    }

    FuncItem* getCommandItems(
        int* count)
    {
        if (count != nullptr)
        {
            *count =
                static_cast<int>(
                    commandItems.size());
        }

        return commandItems.data();
    }

    void commandFormatDocument()
    {
        auto& context =
            PluginContext::instance();

        EditorService editor(
            context.nppData());

        const EditorSnapshot snapshot =
            editor.captureSnapshot();

        try
        {
            const HelperResponse response =
                runHelper(
                    Operation::Format,
                    snapshot.utf8Text);

            if (!response.success)
            {
                Diagnostics(
                    context.nppData())
                    .show(response.diagnostics);

                return;
            }

            const std::string formattedText =
                response.result
                    .at("formatted")
                    .get<std::string>();

            editor.replaceDocument(
                formattedText,
                snapshot);

            setCurrentLanguageToHtml();
        }
        catch (const std::exception& exception)
        {
            showException(exception);
        }
    }

    void commandFormatSelection()
    {
        auto& context =
            PluginContext::instance();

        EditorService editor(
            context.nppData());

        const EditorSnapshot snapshot =
            editor.captureSnapshot();

        const std::string selectedText =
            editor.getSelectedTextUtf8();

        if (selectedText.empty())
        {
            Diagnostics(
                context.nppData())
                .showInformationMessage(
                    L"Jinja2 Tools",
                    L"No text is selected.");

            return;
        }

        try
        {
            const HelperResponse response =
                runHelper(
                    Operation::Format,
                    selectedText);

            if (!response.success)
            {
                Diagnostics(
                    context.nppData())
                    .show(response.diagnostics);

                return;
            }

            const std::string formattedText =
                response.result
                    .at("formatted")
                    .get<std::string>();

            editor.replaceSelection(
                formattedText,
                snapshot);

            setCurrentLanguageToHtml();
        }
        catch (const std::exception& exception)
        {
            showException(exception);
        }
    }

    void commandCheckSyntax()
    {
        auto& context =
            PluginContext::instance();

        EditorService editor(
            context.nppData());

        try
        {
            const HelperResponse response =
                runHelper(
                    Operation::Validate,
                    editor.getDocumentTextUtf8());

            if (response.success)
            {
                Diagnostics(
                    context.nppData())
                    .showInformationMessage(
                        L"Jinja2 Tools",
                        L"Template syntax is valid.");
            }
            else
            {
                Diagnostics(
                    context.nppData())
                    .show(response.diagnostics);
            }
        }
        catch (const std::exception& exception)
        {
            showException(exception);
        }
    }

    void commandAnalyzeTemplate()
    {
        auto& context =
            PluginContext::instance();

        EditorService editor(
            context.nppData());

        try
        {
            const HelperResponse response =
                runHelper(
                    Operation::Analyze,
                    editor.getDocumentTextUtf8());

            if (!response.success)
            {
                Diagnostics(
                    context.nppData())
                    .show(response.diagnostics);

                return;
            }

            Diagnostics(
                context.nppData())
                .showInformationMessage(
                    L"Jinja2 Tools",
                    fromUtf8(
                        response.result.dump(2)));
        }
        catch (const std::exception& exception)
        {
            showException(exception);
        }
    }

    void commandClearDiagnostics()
    {
        auto& context =
            PluginContext::instance();

        EditorService(
            context.nppData())
            .clearDiagnostics();
    }

    void commandAbout()
    {
        auto& context =
            PluginContext::instance();

        MessageBoxW(
            context.nppData()._nppHandle,
            L"Jinja2 Tools " JINJA2TOOLS_VERSION,
            L"About",
            MB_OK | MB_ICONINFORMATION);
    }
}
#pragma once
#include "PluginApi.h"
#include <NotepadPlusPlus/Scintilla.h>
#include <NotepadPlusPlus/Sci_Position.h>
#include <cstdint>
#include <string>
namespace jinja2tools { struct EditorSelection{Sci_Position start{},end{};};struct EditorSnapshot{std::string utf8Text;EditorSelection selection;Sci_Position currentPosition{},firstVisibleLine{};std::uint64_t modificationSequence{};};class EditorService final{public:explicit EditorService(const NppData&);HWND currentScintillaHandle()const;EditorSnapshot captureSnapshot()const;std::string getDocumentTextUtf8()const;std::string getSelectedTextUtf8()const;EditorSelection getSelection()const;void replaceDocument(std::string_view,const EditorSnapshot&);void replaceSelection(std::string_view,const EditorSnapshot&);void clearDiagnostics();private:NppData nppData_;}; }

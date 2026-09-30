#pragma once
#include "PluginApi.h"
namespace jinja2tools{void initializeCommands();FuncItem* getCommandItems(int*);void commandFormatDocument();void commandFormatSelection();void commandCheckSyntax();void commandAnalyzeTemplate();void commandClearDiagnostics();void commandAbout();}

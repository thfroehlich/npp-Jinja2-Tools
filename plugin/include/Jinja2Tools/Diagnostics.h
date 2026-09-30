#pragma once
#include "PluginApi.h"
#include "Protocol.h"
#include <string_view>
namespace jinja2tools { class Diagnostics final{public:explicit Diagnostics(const NppData&);void clear();void show(const std::vector<Diagnostic>&);void showErrorMessage(std::wstring_view,std::wstring_view);void showInformationMessage(std::wstring_view,std::wstring_view);private:NppData nppData_;}; }

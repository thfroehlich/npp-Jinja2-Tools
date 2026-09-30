#pragma once
#include "PluginApi.h"
#include <filesystem>
namespace jinja2tools { class PluginContext final { public: static PluginContext& instance(); void initialize(HMODULE,const NppData&); HMODULE moduleHandle() const noexcept; const NppData& nppData() const noexcept; std::filesystem::path pluginDirectory() const; std::filesystem::path helperExecutable() const; private: PluginContext()=default; HMODULE moduleHandle_{}; NppData nppData_{}; }; }

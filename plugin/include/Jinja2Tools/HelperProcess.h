#pragma once
#include <windows.h>
#include <chrono>
#include <filesystem>
#include <string>
namespace jinja2tools { struct ProcessResult{DWORD exitCode{};std::string standardOutput,standardError;bool timedOut{};}; class HelperProcess final { public: ProcessResult execute(const std::filesystem::path&,std::string_view,std::chrono::milliseconds) const; }; }

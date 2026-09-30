#pragma once
#include <windows.h>
#include <stdexcept>
#include <string>
#include <string_view>
namespace jinja2tools{std::wstring formatWin32Error(DWORD);std::runtime_error makeWin32Error(std::string_view,DWORD);}

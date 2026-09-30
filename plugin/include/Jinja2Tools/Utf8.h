#pragma once
#include <string>
#include <string_view>
namespace jinja2tools { std::string toUtf8(std::wstring_view);std::wstring fromUtf8(std::string_view);}

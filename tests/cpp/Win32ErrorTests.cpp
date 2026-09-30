#include <catch2/catch_test_macros.hpp>
#include "Jinja2Tools/Win32Error.h"
TEST_CASE("Win32 errors have text"){REQUIRE_FALSE(jinja2tools::formatWin32Error(ERROR_FILE_NOT_FOUND).empty());}

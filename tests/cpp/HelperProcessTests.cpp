#include <catch2/catch_test_macros.hpp>
#include "Jinja2Tools/HelperProcess.h"
TEST_CASE("Process result defaults are safe"){jinja2tools::ProcessResult r;REQUIRE_FALSE(r.timedOut);REQUIRE(r.exitCode==0);}

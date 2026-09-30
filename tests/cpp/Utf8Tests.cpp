#include <catch2/catch_test_macros.hpp>
#include "Jinja2Tools/Utf8.h"
TEST_CASE("UTF conversion roundtrips"){std::wstring w=L"Grüße 世界";REQUIRE(jinja2tools::fromUtf8(jinja2tools::toUtf8(w))==w);}

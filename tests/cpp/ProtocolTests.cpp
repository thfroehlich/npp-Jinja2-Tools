#include <catch2/catch_test_macros.hpp>
#include "Jinja2Tools/Protocol.h"
TEST_CASE("JSON safely serializes control characters"){std::string s;for(char c=0;c<32;++c)s.push_back(c);jinja2tools::HelperRequest r{1,"id",jinja2tools::Operation::Format,{"x",s}};auto d=nlohmann::json(r).dump();auto p=nlohmann::json::parse(d);REQUIRE(p["document"]["text"].get<std::string>()==s);}

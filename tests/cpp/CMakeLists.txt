find_package(Catch2 3 CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(
    Jinja2ToolsTests
    ProtocolTests.cpp
    Utf8Tests.cpp
    Win32ErrorTests.cpp
    HelperProcessTests.cpp

    ../../plugin/src/Protocol.cpp
    ../../plugin/src/Utf8.cpp
    ../../plugin/src/Win32Error.cpp
    ../../plugin/src/HelperProcess.cpp
)

target_include_directories(
    Jinja2ToolsTests
    PRIVATE
    ../../plugin/include
)

target_link_libraries(
    Jinja2ToolsTests
    PRIVATE
    Catch2::Catch2WithMain
    nlohmann_json::nlohmann_json
    Rpcrt4
)

include(Catch)

catch_discover_tests(
    Jinja2ToolsTests
)
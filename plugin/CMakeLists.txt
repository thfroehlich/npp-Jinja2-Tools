find_package(nlohmann_json CONFIG REQUIRED)
set(NPP_TAG v8.9.8.1)
include(FetchContent)
FetchContent_Declare(npp_headers URL https://github.com/notepad-plus-plus/notepad-plus-plus/archive/refs/tags/${NPP_TAG}.zip)
FetchContent_MakeAvailable(npp_headers)
add_library(Jinja2Tools SHARED
 resources/Jinja2Tools.rc src/DllMain.cpp src/PluginExports.cpp src/PluginContext.cpp src/PluginCommands.cpp
 src/EditorService.cpp src/HelperClient.cpp src/HelperProcess.cpp src/Protocol.cpp src/Diagnostics.cpp src/Utf8.cpp src/Win32Error.cpp)
target_include_directories(Jinja2Tools PRIVATE include
 ${npp_headers_SOURCE_DIR}/PowerEditor/src/MISC/PluginsManager
 ${npp_headers_SOURCE_DIR}/PowerEditor/src
 ${npp_headers_SOURCE_DIR}/scintilla/include)
target_link_libraries(Jinja2Tools PRIVATE nlohmann_json::nlohmann_json user32)
target_compile_definitions(Jinja2Tools PRIVATE UNICODE _UNICODE WIN32_LEAN_AND_MEAN NOMINMAX)
target_compile_options(Jinja2Tools PRIVATE /W4 /permissive- /EHsc /utf-8)
target_link_options(Jinja2Tools PRIVATE /DEF:${CMAKE_CURRENT_SOURCE_DIR}/src/exports.def)
set_target_properties(Jinja2Tools PROPERTIES PREFIX "")
if(TARGET Jinja2ToolsHelper)
    add_dependencies(
        Jinja2Tools
        Jinja2ToolsHelper
    )
endif()
install(TARGETS Jinja2Tools RUNTIME DESTINATION Jinja2Tools)

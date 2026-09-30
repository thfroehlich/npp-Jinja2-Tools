if(JINJA2TOOLS_BUILD_HELPER)
  find_program(POWERSHELL_EXECUTABLE
    NAMES pwsh powershell
    REQUIRED
  )

  add_custom_target(
    Jinja2ToolsHelper
    COMMAND
        ${POWERSHELL_EXECUTABLE}
        -NoProfile
        -ExecutionPolicy Bypass
        -File
        "${CMAKE_SOURCE_DIR}/helper/build-helper.ps1"
    WORKING_DIRECTORY
        "${CMAKE_SOURCE_DIR}/helper"
    VERBATIM
  )
endif()

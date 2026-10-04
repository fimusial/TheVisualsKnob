cmake_minimum_required(VERSION 3.14.0)
include(cmrc/CMakeRC.cmake)

option(SMTG_ENABLE_VST3_PLUGIN_EXAMPLES "Enable VST 3 Plug-in Examples" OFF)
option(SMTG_ENABLE_VST3_HOSTING_EXAMPLES "Enable VST 3 Hosting Examples" OFF)

set(CMAKE_OSX_DEPLOYMENT_TARGET 10.13 CACHE STRING "")
set(vst3sdk_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/../../vst3sdk")

if(NOT vst3sdk_SOURCE_DIR)
    message(FATAL_ERROR "Path to VST3 SDK is empty!")
endif()

project(TheVisualsKnob
    VERSION 1.0.0.0 
    DESCRIPTION "TheVisualsKnob VST 3 Plug-in"
)

set(SMTG_VSTGUI_ROOT "${vst3sdk_SOURCE_DIR}")
add_subdirectory(${vst3sdk_SOURCE_DIR} ${PROJECT_BINARY_DIR}/vst3sdk)
smtg_enable_vst3_sdk()

smtg_add_vst3plugin(TheVisualsKnob
    resource/uidesc.json
    source/bufferfifo.h
    source/cids.h
    source/controller.cpp
    source/controller.h
    source/entry.cpp
    source/processor.cpp
    source/processor.h
    source/resourcemanager.h
    source/version.h
)

cmrc_add_resource_library(compiled_resources NAMESPACE compiled_resources
    resource/uidesc.json
)

target_link_libraries(TheVisualsKnob
    PRIVATE
        compiled_resources
)

if(SMTG_ENABLE_VSTGUI_SUPPORT)
    target_link_libraries(TheVisualsKnob
        PRIVATE
            vstgui_support
    )
endif(SMTG_ENABLE_VSTGUI_SUPPORT)

target_link_libraries(TheVisualsKnob
    PRIVATE
        sdk
)

smtg_target_configure_version_file(TheVisualsKnob)

if(MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT TheVisualsKnob)

    smtg_target_set_debug_executable(TheVisualsKnob
        "$(ProgramW6432)/Steinberg/VST3PluginTestHost/VST3PluginTestHost.exe"
        "--pluginfolder \"$(OutDir)/\""
    )
endif()

source_group("Header Files" REGULAR_EXPRESSION ".*/source/.*\\.h$")
source_group("Source Files" REGULAR_EXPRESSION ".*/source/.*\\.cpp$")
source_group("Resources" REGULAR_EXPRESSION ".*/resource/.*$")

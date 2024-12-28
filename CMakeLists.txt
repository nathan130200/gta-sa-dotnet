cmake_minimum_required(VERSION 3.29)

project("GTA")

set(PLUGIN_NAME dotnet_loader)

add_library(${PLUGIN_NAME} SHARED
    src/DllMain.cpp
)

target_link_libraries(${PLUGIN_NAME}
    $<$<CONFIG:Release>:plugin.lib>
    $<$<CONFIG:Debug>:plugin_d.lib>
    "nethost.lib"
)

target_include_directories(${PLUGIN_NAME} 
    PRIVATE
        "$ENV{PLUGIN_SDK_DIR}/plugin_SA"
        "$ENV{PLUGIN_SDK_DIR}/plugin_SA/game_SA"
        "$ENV{PLUGIN_SDK_DIR}/shared"
        "$ENV{PLUGIN_SDK_DIR}/shared/game"
        "${PROJECT_SOURCE_DIR}/vendor/coreclr"
)

target_link_directories(${PLUGIN_NAME} 
    PRIVATE
        "$ENV{DXSDK_DIR}/Lib/x86"
        "$ENV{PLUGIN_SDK_DIR}/output/lib"
        "$ENV{PLUGIN_SDK_DIR}/shared/bass"
        "${PROJECT_SOURCE_DIR}/vendor/coreclr"
)

target_compile_definitions(${PLUGIN_NAME} 
    PRIVATE
        _CRT_SECURE_NO_WARNINGS
        _CRT_NON_CONFORMING_SWPRINTFS
        _USE_MATH_DEFINES
        _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
        GTASA
        GAMESA
        PLUGIN_SGV_10US
        RW
)

set_target_properties(${PLUGIN_NAME} PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED On
    CXX_EXTENSIONS On
    SUFFIX ".asi"
)

target_compile_options(${PLUGIN_NAME} PRIVATE
    /std:c++latest 
    /source-charset:windows-1252
    $<$<CONFIG:Debug>:/MTd>
    $<$<CONFIG:Release>:/MT>
)

add_custom_command(TARGET ${PLUGIN_NAME} PRE_BUILD
    COMMAND "cmd.exe" ARGS /c taskkill /im gta_sa.exe /F /FI "STATUS eq RUNNING"
)

add_custom_command(TARGET ${PLUGIN_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PLUGIN_NAME}> $ENV{GTA_SA_DIR}        
)

cmake_minimum_required(VERSION 3.28)
project(Lumadi_Configs)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

include(FetchContent)

FetchContent_Declare(
        json
        URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
)

FetchContent_MakeAvailable(json)

add_library(Lumadi_Configs STATIC)

target_sources(Lumadi_Configs
        PUBLIC
        FILE_SET CXX_MODULES FILES
        src/SettingTypes.cppm
        src/Configs.cppm
)
set_target_properties(Lumadi_Configs PROPERTIES CXX_SCAN_FOR_MODULES ON)

target_link_libraries(Lumadi_Configs PUBLIC
        nlohmann_json::nlohmann_json
)
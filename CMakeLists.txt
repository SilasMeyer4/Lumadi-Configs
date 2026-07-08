cmake_minimum_required(VERSION 4.2)
project(Lumadi_Configs)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)

FetchContent_Declare(
        json
        URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
)

FetchContent_MakeAvailable(json)

add_library(Lumadi_Configs STATIC
        src/Configs.cpp
        include/lumadi/config/detail/Configs.h
        include/lumadi/config/detail/SettingType.h
        include/lumadi/config/config.h)

target_include_directories(Lumadi_Configs
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(Lumadi_Configs PUBLIC
        nlohmann_json::nlohmann_json
)
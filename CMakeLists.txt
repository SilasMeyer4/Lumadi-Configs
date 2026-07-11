cmake_minimum_required(VERSION 3.28)

project(Lumadi_Configs LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
        json
        URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
)

FetchContent_MakeAvailable(json)

add_library(Lumadi_Configs STATIC
        src/Configs.cpp
)

target_include_directories(Lumadi_Configs
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(Lumadi_Configs
        PUBLIC
        nlohmann_json::nlohmann_json
)

option(ENABLE_TESTS "Enable unit tests" OFF)

if (ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif ()
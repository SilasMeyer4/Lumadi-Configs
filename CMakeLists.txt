cmake_minimum_required(VERSION 4.2)
project(Lumadi_Configs)

set(CMAKE_CXX_STANDARD 23)


add_library(Lumadi_Configs STATIC
        src/Settings.cpp
        include/lumadi/config/detail/Settings.h)

target_include_directories(Lumadi_Configs
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
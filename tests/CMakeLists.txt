include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.tar.gz
)

FetchContent_MakeAvailable(googletest)

add_executable(Lumadi_Tests
        test_setting_types.cpp
        test_setting.cpp
        test_category.cpp
        test_app_settings.cpp
)

target_link_libraries(Lumadi_Tests
        PRIVATE
        Lumadi_Configs
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(Lumadi_Tests)

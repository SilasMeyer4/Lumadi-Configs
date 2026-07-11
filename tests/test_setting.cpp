#include <gtest/gtest.h>
#include "lumadi/configs/details/Configs.h"

using namespace Lumadi::Configs;

TEST(SettingTest, GetName) {
    Setting<int> s("volume", 50, 50);
    EXPECT_EQ(s.GetName(), "volume");
}

TEST(SettingTest, GetDefaultValue) {
    Setting<int> s("volume", 50, 100);
    EXPECT_EQ(s.GetDefaultValue(), 100);
}

TEST(SettingTest, GetInitialValue) {
    Setting<int> s("volume", 50, 100);
    EXPECT_EQ(s.Get(), 50);
}

TEST(SettingTest, SetValueTyped) {
    Setting<int> s("volume", 50, 100);
    s.SetValue(75);
    EXPECT_EQ(s.Get(), 75);
}

TEST(SettingTest, SetValueFromJson) {
    Setting<int> s("volume", 50, 100);
    s.SetValue("80");
    EXPECT_EQ(s.Get(), 80);
}

TEST(SettingTest, SetValueFromInvalidJsonFallsBackToDefault) {
    Setting<int> s("volume", 50, 100);
    s.SetValue("not_a_number");
    EXPECT_EQ(s.Get(), 100);
}

TEST(SettingTest, SetValueFromEmptyJsonFallsBackToDefault) {
    Setting<int> s("volume", 50, 100);
    s.SetValue("");
    EXPECT_EQ(s.Get(), 100);
}

TEST(SettingTest, GetJsonInt) {
    Setting<int> s("volume", 50, 100);
    EXPECT_EQ(s.GetJson(), "50");
}

TEST(SettingTest, GetJsonBool) {
    Setting<bool> s("enabled", true, true);
    EXPECT_EQ(s.GetJson(), "true");
}

TEST(SettingTest, GetJsonFloat) {
    Setting<float> s("ratio", 0.5f, 1.0f);
    EXPECT_EQ(s.GetJson(), "0.5");
}

TEST(SettingTest, GetJsonString) {
    Setting<std::string> s("name", "hello", "default");
    EXPECT_EQ(s.GetJson(), "\"hello\"");
}

TEST(SettingTest, OnChangedCallback) {
    Setting<int> s("volume", 50, 100);
    int received = 0;
    s.OnChanged([&](const int &val) { received = val; });
    s.SetValue(75);
    EXPECT_EQ(received, 75);
}

TEST(SettingTest, OnChangedNotCalledOnConstruction) {
    Setting<int> s("volume", 50, 100);
    bool called = false;
    s.OnChanged([&](const int &) { called = true; });
    EXPECT_FALSE(called);
}

TEST(SettingTest, GetTypeInt) {
    Setting<int> s("x", 0, 0);
    EXPECT_EQ(s.GetType(), SettingType::Int);
}

TEST(SettingTest, GetTypeBool) {
    Setting<bool> s("x", false, false);
    EXPECT_EQ(s.GetType(), SettingType::Bool);
}

TEST(SettingTest, GetTypeString) {
    Setting<std::string> s("x", "", "");
    EXPECT_EQ(s.GetType(), SettingType::String);
}

TEST(SettingTest, GetStringListJson) {
    Setting<std::vector<std::string>> s("tags", {"a", "b"}, {});
    EXPECT_EQ(s.GetJson(), R"(["a","b"])");
}

TEST(SettingTest, SetStringListFromJson) {
    Setting<std::vector<std::string>> s("tags", {"a", "b"}, {});
    s.SetValue(R"(["x","y","z"])");
    auto expected = std::vector<std::string>{"x", "y", "z"};
    EXPECT_EQ(s.Get(), expected);
}

TEST(SettingTest, SetValueFromJsonInvalidTypeFallsBackToDefault) {
    Setting<int> s("volume", 50, 100);
    s.SetValue(R"("not_an_int")");
    EXPECT_EQ(s.Get(), 100);
}

#include <gtest/gtest.h>
#include "lumadi/configs/details/Configs.h"

using namespace Lumadi::Configs;

TEST(CategoryTest, GetName) {
    AppSettingCategory cat("audio");
    EXPECT_EQ(cat.GetName(), "audio");
}

TEST(CategoryTest, CreateSetting) {
    AppSettingCategory cat("audio");
    auto &s = cat.CreateSetting<int>("volume", 50);
    EXPECT_EQ(s.Get(), 50);
    EXPECT_EQ(s.GetName(), "volume");
}

TEST(CategoryTest, GetSettingExists) {
    AppSettingCategory cat("audio");
    cat.CreateSetting<int>("volume", 50);
    auto *s = cat.GetSetting<int>("volume");
    ASSERT_NE(s, nullptr);
    EXPECT_EQ(s->Get(), 50);
}

TEST(CategoryTest, GetSettingMissing) {
    AppSettingCategory cat("audio");
    auto *s = cat.GetSetting<int>("nonexistent");
    EXPECT_EQ(s, nullptr);
}

TEST(CategoryTest, GetSettingsList) {
    AppSettingCategory cat("audio");
    cat.CreateSetting<int>("volume", 50);
    cat.CreateSetting<bool>("muted", false);
    EXPECT_EQ(cat.GetSettings().size(), 2u);
}

TEST(CategoryTest, MultipleSettingsDifferentTypes) {
    AppSettingCategory cat("graphics");
    auto &intSetting = cat.CreateSetting<int>("resolution", 1080);
    auto &boolSetting = cat.CreateSetting<bool>("fullscreen", true);
    auto &floatSetting = cat.CreateSetting<float>("brightness", 0.8f);
    auto &stringSetting = cat.CreateSetting<std::string>("backend", "vulkan");

    EXPECT_EQ(intSetting.Get(), 1080);
    EXPECT_EQ(boolSetting.Get(), true);
    EXPECT_FLOAT_EQ(floatSetting.Get(), 0.8f);
    EXPECT_EQ(stringSetting.Get(), "vulkan");
}

TEST(CategoryTest, CreateSettingWithDescription) {
    AppSettingCategory cat("audio", "Audio configuration settings");
    auto &s = cat.CreateSetting<int>("volume", 50);
    EXPECT_EQ(s.Get(), 50);
    EXPECT_EQ(cat.GetName(), "audio");
}

TEST(CategoryTest, CallbackThroughCategory) {
    AppSettingCategory cat("audio");
    auto &s = cat.CreateSetting<int>("volume", 50);
    int received = 0;
    s.OnChanged([&](const int &val) { received = val; });
    s.SetValue(80);
    EXPECT_EQ(received, 80);
}

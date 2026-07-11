#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include "lumadi/configs/details/Configs.h"

using namespace Lumadi::Configs;

class AppSettingsTest : public ::testing::Test {
protected:
    std::filesystem::path testDir;

    void SetUp() override {
        testDir = std::filesystem::temp_directory_path() / "lumadi_test";
        std::filesystem::create_directories(testDir);
    }

    void TearDown() override {
        std::filesystem::remove_all(testDir);
    }

    std::filesystem::path testFile(const std::string &name) {
        return testDir / name;
    }
};

TEST_F(AppSettingsTest, CreateCategory) {
    AppSettings settings(testFile("test.json").string());
    auto &cat = settings.CreateCategory("audio");
    EXPECT_EQ(cat.GetName(), "audio");
}

TEST_F(AppSettingsTest, GetCategoryExists) {
    AppSettings settings(testFile("test.json").string());
    settings.CreateCategory("audio");
    auto *cat = settings.GetCategory("audio");
    ASSERT_NE(cat, nullptr);
    EXPECT_EQ(cat->GetName(), "audio");
}

TEST_F(AppSettingsTest, GetCategoryMissing) {
    AppSettings settings(testFile("test.json").string());
    auto *cat = settings.GetCategory("nonexistent");
    EXPECT_EQ(cat, nullptr);
}

TEST_F(AppSettingsTest, GetSettingTwoLevel) {
    AppSettings settings(testFile("test.json").string());
    auto &cat = settings.CreateCategory("audio");
    cat.CreateSetting<int>("volume", 50);
    auto *s = settings.GetSetting<int>("audio", "volume");
    ASSERT_NE(s, nullptr);
    EXPECT_EQ(s->Get(), 50);
}

TEST_F(AppSettingsTest, GetSettingInvalidCategory) {
    AppSettings settings(testFile("test.json").string());
    auto *s = settings.GetSetting<int>("nonexistent", "volume");
    EXPECT_EQ(s, nullptr);
}

TEST_F(AppSettingsTest, SetPath) {
    AppSettings settings(testFile("test.json").string());
    auto newPath = testFile("other.json").string();
    settings.SetPath(newPath);
    auto &cat = settings.CreateCategory("audio");
    cat.CreateSetting<int>("volume", 50);
    settings.Save();
    EXPECT_TRUE(std::filesystem::exists(testFile("other.json")));
}

TEST_F(AppSettingsTest, SaveCreatesFile) {
    AppSettings settings(testFile("subdir/test.json").string());
    auto &cat = settings.CreateCategory("audio");
    cat.CreateSetting<int>("volume", 50);
    settings.Save();
    EXPECT_TRUE(std::filesystem::exists(testFile("subdir/test.json")));
}

TEST_F(AppSettingsTest, SaveContentValid) {
    AppSettings settings(testFile("test.json").string(), "1.0.0");
    auto &cat = settings.CreateCategory("audio");
    cat.CreateSetting<int>("volume", 50);
    cat.CreateSetting<bool>("muted", false);
    settings.Save();

    std::ifstream file(testFile("test.json"));
    ASSERT_TRUE(file.is_open());
    nlohmann::json root;
    file >> root;

    EXPECT_EQ(root["version"], "1.0.0");
    EXPECT_EQ(root["audio"]["volume"], 50);
    EXPECT_EQ(root["audio"]["muted"], false);
}

TEST_F(AppSettingsTest, LoadRestoresValues) {
    auto path = testFile("test.json").string();
    {
        AppSettings settings(path, "2.0.0");
        auto &cat = settings.CreateCategory("audio");
        cat.CreateSetting<int>("volume", 75);
        cat.CreateSetting<std::string>("device", "speakers");
        settings.Save();
    }

    AppSettings settings2(path);
    auto &cat = settings2.CreateCategory("audio");
    cat.CreateSetting<int>("volume", 0);
    cat.CreateSetting<std::string>("device", "");
    settings2.Load();

    auto *vol = settings2.GetSetting<int>("audio", "volume");
    auto *dev = settings2.GetSetting<std::string>("audio", "device");
    ASSERT_NE(vol, nullptr);
    ASSERT_NE(dev, nullptr);
    EXPECT_EQ(vol->Get(), 75);
    EXPECT_EQ(dev->Get(), "speakers");
}

TEST_F(AppSettingsTest, LoadMissingFileSavesDefaults) {
    auto path = testFile("nonexistent/test.json").string();
    AppSettings settings(path);
    auto &cat = settings.CreateCategory("audio");
    cat.CreateSetting<int>("volume", 50);
    settings.Load();
    EXPECT_TRUE(std::filesystem::exists(testFile("nonexistent/test.json")));
}

TEST_F(AppSettingsTest, LoadMalformedJsonNoCrash) {
    auto path = testFile("test.json").string();
    {
        std::ofstream file(path);
        file << "{invalid json content!!!";
    }
    AppSettings settings(path);
    auto &cat = settings.CreateCategory("audio");
    cat.CreateSetting<int>("volume", 50);
    settings.Load();
    EXPECT_EQ(cat.GetSetting<int>("volume")->Get(), 50);
}

TEST_F(AppSettingsTest, GetJson) {
    AppSettings settings(testFile("test.json").string(), "1.0.0");
    auto &cat = settings.CreateCategory("audio");
    cat.CreateSetting<int>("volume", 50);
    std::string json = settings.GetJson();
    EXPECT_NE(json.find("\"version\""), std::string::npos);
    EXPECT_NE(json.find("\"audio\""), std::string::npos);
    EXPECT_NE(json.find("\"volume\""), std::string::npos);
}

TEST_F(AppSettingsTest, VersionStringPersisted) {
    auto path = testFile("test.json").string();
    {
        AppSettings settings(path, "3.1.4");
        settings.Save();
    }
    AppSettings settings2(path);
    settings2.Load();
    nlohmann::json root;
    std::ifstream file(path);
    file >> root;
    EXPECT_EQ(root["version"], "3.1.4");
}

TEST_F(AppSettingsTest, SaveAndLoadMultipleCategories) {
    auto path = testFile("test.json").string();
    {
        AppSettings settings(path);
        auto &audio = settings.CreateCategory("audio");
        audio.CreateSetting<int>("volume", 80);
        auto &video = settings.CreateCategory("video");
        video.CreateSetting<bool>("vsync", true);
        settings.Save();
    }

    AppSettings settings2(path);
    auto &audio2 = settings2.CreateCategory("audio");
    audio2.CreateSetting<int>("volume", 0);
    auto &video2 = settings2.CreateCategory("video");
    video2.CreateSetting<bool>("vsync", false);
    settings2.Load();

    EXPECT_EQ(audio2.GetSetting<int>("volume")->Get(), 80);
    EXPECT_EQ(video2.GetSetting<bool>("vsync")->Get(), true);
}

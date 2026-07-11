#include <gtest/gtest.h>
#include "lumadi/configs/details/SettingTypes.h"

using namespace Lumadi::Configs;

TEST(SettingTypeTraits, Bool) {
    EXPECT_EQ(SettingTypeTraits<bool>::value, SettingType::Bool);
}

TEST(SettingTypeTraits, Int) {
    EXPECT_EQ(SettingTypeTraits<int32_t>::value, SettingType::Int);
}

TEST(SettingTypeTraits, Float) {
    EXPECT_EQ(SettingTypeTraits<float>::value, SettingType::Float);
}

TEST(SettingTypeTraits, String) {
    EXPECT_EQ(SettingTypeTraits<std::string>::value, SettingType::String);
}

TEST(SettingTypeTraits, StringList) {
    EXPECT_EQ(SettingTypeTraits<std::vector<std::string>>::value, SettingType::StringList);
}

TEST(SettingTypeTraits, IntList) {
    EXPECT_EQ(SettingTypeTraits<std::vector<int32_t>>::value, SettingType::IntList);
}

TEST(SettingTypeTraits, FloatList) {
    EXPECT_EQ(SettingTypeTraits<std::vector<float>>::value, SettingType::FloatList);
}

struct UnknownType {};
TEST(SettingTypeTraits, Unknown) {
    EXPECT_EQ(SettingTypeTraits<UnknownType>::value, SettingType::Undefined);
}

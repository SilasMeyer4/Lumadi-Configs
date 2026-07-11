//
// Created by silas on 11.07.26.
//

#ifndef LUMADI_CONFIGS_SETTINGTYPES_H
#define LUMADI_CONFIGS_SETTINGTYPES_H

#include <vector>
#include <string>
#include <cstdint>

namespace Lumadi::Configs {
    enum class SettingType {
        Undefined,
        Bool,
        Int,
        Float,
        String,
        StringList,
        IntList,
        FloatList,
    };

    template<typename T>
    struct SettingTypeTraits {
        static constexpr SettingType value = SettingType::Undefined;
    };

    template<>
    struct SettingTypeTraits<bool> {
        static constexpr SettingType value = SettingType::Bool;
    };

    template<>
    struct SettingTypeTraits<int32_t> {
        static constexpr SettingType value = SettingType::Int;
    };

    template<>
    struct SettingTypeTraits<float> {
        static constexpr SettingType value = SettingType::Float;
    };

    template<>
    struct SettingTypeTraits<std::string> {
        static constexpr SettingType value = SettingType::String;
    };

    template<>
    struct SettingTypeTraits<std::vector<std::string> > {
        static constexpr SettingType value = SettingType::StringList;
    };

    template<>
    struct SettingTypeTraits<std::vector<std::int32_t> > {
        static constexpr SettingType value = SettingType::IntList;
    };

    template<>
    struct SettingTypeTraits<std::vector<float> > {
        static constexpr SettingType value = SettingType::FloatList;
    };
}


#endif //LUMADI_CONFIGS_SETTINGTYPES_H

//
// Created by silas on 11.07.26.
//

#ifndef LUMADI_CONFIGS_DETAILS_CONFIGS_H
#define LUMADI_CONFIGS_DETAILS_CONFIGS_H

#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <utility>
#include <filesystem>
#include "nlohmann/json.hpp"
#include "SettingTypes.h"

namespace Lumadi::Configs {
  class ISetting {
  public:
    virtual ~ISetting() = default;

    [[nodiscard]] virtual std::string_view GetName() const = 0;

    [[nodiscard]] virtual std::string GetJson() const = 0;

    virtual void SetValue(const std::string &jsonString) = 0;

    [[nodiscard]] virtual SettingType GetType() const = 0;
  };


  template<typename T>
  class Setting : public ISetting {
  public:
    using Callback = std::function<void(const T &)>;

    Setting(std::string name, T value, T defaultValue) : mName(std::move(name)), mValue(value),
                                                         mDefaultValue(defaultValue) {
    }

    [[nodiscard]] std::string_view GetName() const override {
      return mName;
    }

    void SetValue(const std::string &jsonString) override {
      try {
        auto value = nlohmann::json::parse(jsonString);
        mValue = value.get<T>();
      } catch (...) {
        mValue = mDefaultValue;
      }
      NotifyChanged();
    }

    [[nodiscard]] std::string GetJson() const override {
      return nlohmann::json(mValue).dump();
    }

    void SetValue(T value) {
      mValue = std::move(value);
      NotifyChanged();
    }

    const T &Get() const {
      return mValue;
    }

    const T &GetDefaultValue() const {
      return mDefaultValue;
    }

    void OnChanged(Callback callBack) {
      mCallback = std::move(callBack);
    }

    [[nodiscard]] SettingType GetType() const override {
      return SettingTypeTraits<T>::value;
    }

  private:
    std::string mName;
    T mValue;
    T mDefaultValue;
    Callback mCallback;

    void NotifyChanged() {
      if (mCallback) {
        mCallback(mValue);
      }
    }
  };

  class IAppSettingCategory {
  public:
    virtual ~IAppSettingCategory() = default;

    virtual std::vector<std::unique_ptr<ISetting> > &GetSettings() = 0;

    [[nodiscard]] virtual std::string_view GetName() const = 0;
  };

  class AppSettingCategory : public IAppSettingCategory {
  public:
    explicit AppSettingCategory(std::string name) : mName(std::move(name)) {
    }

    AppSettingCategory(std::string name, std::string description) : mName(std::move(name)),
                                                                    mDescription(std::move(description)) {
    }


    template<typename T>
    Setting<T> &CreateSetting(std::string name, T defaultValue) {
      auto option = std::make_unique<Setting<T> >(std::move(name), defaultValue, defaultValue);
      auto *ptr = option.get();
      mSettings.push_back(std::move(option));
      return *ptr;
    }

    template<typename T>
    Setting<T> *GetSetting(const std::string &name) {
      auto it = std::find_if(mSettings.begin(), mSettings.end(), [&](const auto &setting) {
        return setting->GetName() == name;
      });

      if (it == mSettings.end()) {
        return nullptr;
      }

      ISetting *rawInterfacePtr = it->get();
      return static_cast<Setting<T> *>(rawInterfacePtr);
    }

    std::vector<std::unique_ptr<ISetting> > &GetSettings() override {
      return mSettings;
    }

    [[nodiscard]] std::string_view GetName() const override {
      return mName;
    }

  private:
    std::string mName;
    std::string mDescription;
    std::vector<std::unique_ptr<ISetting> > mSettings;
  };

  class AppSettings {
  public:
    AppSettings(std::string path, std::string versionString = "");
    void Save() const;
    void Load();
    [[nodiscard]] std::string GetJson() const;
    void SetPath(std::string path);
    AppSettingCategory *GetCategory(const std::string &name);
    std::vector<std::unique_ptr<IAppSettingCategory> > &GetCategories();

    template<typename T>
    Setting<T> *GetSetting(const std::string &category, const std::string &name) {
      if (const auto cat = GetCategory(category)) {
        return cat->GetSetting<T>(name);
      }

      return nullptr;
    }

    template<typename T = AppSettingCategory, typename... Args>
    T &CreateCategory(Args &&... args) {
      static_assert(std::is_base_of_v<AppSettingCategory, T>,
                    "Needs to inherit from AppSettingCategory!");

      auto category = std::make_unique<T>(std::forward<Args>(args)...);
      auto *ptr = category.get();
      mCategories.push_back(std::move(category));
      return *ptr;
    }

  private:
    [[nodiscard]] nlohmann::json ToJson() const;

    std::vector<std::unique_ptr<IAppSettingCategory> > mCategories;
    std::string mPath;
    std::string mVersionString;
  };
}

#endif //LUMADI_CONFIGS_DETAILS_CONFIGS_H

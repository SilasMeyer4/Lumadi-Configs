//
// Created by meyer on 08.07.2026.
//

#ifndef LUMADI_CONFIGS_SETTINGS_H
#define LUMADI_CONFIGS_SETTINGS_H

#include <string>
#include <vector>
#include <functional>
#include <memory>
#include "nlohmann/json.hpp"
#include "SettingType.h"

class ISetting
{
public:
  virtual ~ISetting() = default;
  [[nodiscard]] virtual std::string_view GetName() const = 0;
  [[nodiscard]] virtual nlohmann::json GetJson() const = 0;
  virtual void SetValue(const nlohmann::json &value) = 0;
  [[nodiscard]] virtual SettingType GetType() const = 0;
};


template <typename T>
class Setting : public ISetting
{
public:
  using Callback = std::function<void(const T &)>;

  Setting(std::string name, T value, T defaultValue) : mName(std::move(name)), mValue(value), mDefaultValue(defaultValue)
  {
  }

  [[nodiscard]] std::string_view GetName() const override
  {
    return mName;
  }

  void SetValue(const nlohmann::json &value) override
  {
    SetValue(value.get<T>());
  }

  [[nodiscard]] nlohmann::json GetJson() const override
  {
    return mValue;
  }

  void SetValue(T value)
  {
    mValue = std::move(value);

    if (mCallback)
    {
      mCallback(mValue);
    }
  }

  const T &Get() const
  {
    return mValue;
  }

  const T &GetDefaultValue() const
  {
    return mDefaultValue;
  }

  void OnChanged(Callback callBack)
  {
    mCallback = std::move(callBack);
  }

  [[nodiscard]] SettingType GetType() const override
  {
    return SettingType::Undefined;
  }

private:
  std::string mName;
  T mValue;
  T mDefaultValue;
  Callback mCallback;
};

class IAppConfigCategory
{
public:
  virtual ~IAppConfigCategory() = default;
  virtual std::vector<std::unique_ptr<ISetting>> &GetSettings() = 0;
  [[nodiscard]] virtual std::string_view GetName() const = 0;
};

class AppConfigCategory : public IAppConfigCategory
{
  public:
  explicit AppConfigCategory(std::string name) : mName(std::move(name)){}
  AppConfigCategory(std::string name, std::string description) : mName(std::move(name)), mDescription(std::move(description)){}


  template<typename T>
  Setting<T>& CreateSetting(std::string name, T defaultValue)
  {
    auto option = std::make_unique<Setting<T>>(std::move(name), defaultValue, defaultValue);
    auto* ptr = option.get();
    mSettings.push_back(std::move(option));
    return *ptr;
  }

  std::vector<std::unique_ptr<ISetting>>& GetSettings() override
  {
    return mSettings;
  }

  [[nodiscard]] std::string_view GetName() const override
  {
    return mName;
  }

  private:
  std::string mName;
  std::string mDescription;
  std::vector<std::unique_ptr<ISetting>> mSettings;
};

class AppSettings
{
public:
  AppSettings(std::string path, std::string versionString = "");
  void Save();
  void Load();
  [[nodiscard]] std::string GetJson() const;
  void SetPath(std::string path);

  template<typename T = AppConfigCategory, typename... Args>
  T& CreateCategory(Args&&... args)
  {
    static_assert(std::is_base_of_v<AppConfigCategory, T>,
                  "Needs to inherit from AppConfigCategory!");

    auto category = std::make_unique<T>(std::forward<Args>(args)...);
    auto* ptr = category.get();
    mCategories.push_back(std::move(category));
    return *ptr;
  }

private:
  [[nodiscard]] nlohmann::json ToJson() const;
  std::vector<std::unique_ptr<IAppConfigCategory>> mCategories;
  std::string mPath;
  std::string mVersionString;
};
#endif //LUMADI_CONFIGS_SETTINGS_H

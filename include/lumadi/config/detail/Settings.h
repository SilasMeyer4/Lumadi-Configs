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

typedef struct Settings
{

} Settings;

class IOption
{
public:
  virtual ~IOption() = default;
  virtual std::string_view GetName() const = 0;
  virtual void SetFromJson(const nlohmann::json &value) = 0;
  virtual nlohmann::json GetJson() const = 0;
};

template <typename T>
class Option : public IOption
{
public:
  using Callback = std::function<void(const T &)>;

  Option(std::string name, T value) : mName(std::move(name)), mValue(value)
  {
  }

  virtual std::string_view GetName() const override
  {
    return mName;
  }

  virtual void SetFromJson(const nlohmann::json &value) override
  {
    SetValue(value.get<T>());
  }

  nlohmann::json GetJson() const override
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

  void OnChanged(Callback callBack)
  {
    mCallback = std::move(callBack);
  }

private:
  std::string mName;
  T mValue;
  Callback mCallback;
};

class IAppSettingCategory
{
public:
  virtual ~IAppSettingCategory() = default;
  virtual std::vector<std::unique_ptr<IOption>> &GetOptions() = 0;
};

class AppSettings
{
public:
  void Save();
  AppSettings *LoadFromPath(std::string &path);

private:
  std::string ToJson();
  std::vector<IAppSettingCategory> mSettings;
};
#endif //LUMADI_CONFIGS_SETTINGS_H

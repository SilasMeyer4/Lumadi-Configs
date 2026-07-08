//
// Created by meyer on 08.07.2026.
//

#include "lumadi/config/detail/Configs.h"

#include <fstream>
#include <utility>

#include "nlohmann/json.hpp"

AppSettings::AppSettings(std::string path, std::string versionString) : mPath(std::move(path)), mVersionString(std::move(versionString))
{
}

void AppSettings::Save()
{
  const auto root = AppSettings::ToJson();

  std::ofstream file(mPath);
  file << root.dump(4);
  file.close();
}

void AppSettings::Load()
{
  std::ifstream file(mPath);

  nlohmann::json root;
  file >> root;

   mVersionString = root.value("version", "");

  for (auto& category : mCategories)
  {
    if (!root.contains(category->GetName()))
      continue;

    auto& cat = root[category->GetName()];

    for (auto& option : category->GetSettings())
    {
      if (!cat.contains(option->GetName()))
        continue;

      option->SetFromJson(cat[option->GetName()]);
    }
  }
  file.close();
}

std::string AppSettings::GetJson() const
{
  return ToJson().dump(4);
}

void AppSettings::SetPath(std::string path)
{
  mPath = std::move(path);
}

nlohmann::json AppSettings::ToJson() const
{
  nlohmann::json root;
  root["version"] = mVersionString;
  for (auto& category : mCategories)
  {
    for (auto& option : category->GetSettings())
    {
      root[category->GetName()][option->GetName()] = option->GetJson();
    }
  }

  return root;
}

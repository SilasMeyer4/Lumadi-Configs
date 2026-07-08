//
// Created by meyer on 08.07.2026.
//

#include "lumadi/config/detail/Configs.h"

#include <fstream>
#include <utility>

#include "nlohmann/json.hpp"

AppSettings::AppSettings(std::string path, std::string versionString) : mPath(std::move(path)),
                                                                        mVersionString(std::move(versionString))
{
}

void AppSettings::Save()
{
  const auto root = AppSettings::ToJson();

  const std::filesystem::path filePath(mPath);
  auto parentPath = filePath.parent_path();

  if (!parentPath.empty() && !std::filesystem::exists(parentPath))
  {
    std::filesystem::create_directories(parentPath);
  }

  std::ofstream file(mPath);
  if (file.is_open())
  {
    file << root.dump(4);
    file.close();
  }

}

void AppSettings::Load()
{
  std::ifstream file(mPath);

  if (!file.is_open())
  {
    Save();
    return;
  }

  nlohmann::json root;
  try
  {
    file >> root;
  } catch (const nlohmann::json::parse_error &e)
  {
    file.close();
    return;
  }

  file.close();


  mVersionString = root.value("version", "");

  for (const auto &category: mCategories)
  {
    if (!root.contains(category->GetName()))
      continue;

    auto &cat = root[category->GetName()];

    for (const auto &option: category->GetSettings())
    {
      if (!cat.contains(option->GetName()))
      {
        continue;
      }

      option->SetValue(cat[option->GetName()]);
    }
  }
}

std::string AppSettings::GetJson() const
{
  return ToJson().dump(4);
}

void AppSettings::SetPath(std::string path)
{
  mPath = std::move(path);
}

AppConfigCategory* AppSettings::GetCategory(const std::string &name)
{
  const auto it = std::ranges::find_if(mCategories, [&](const auto &category)
  {
    return category->GetName() == name;
  });

  if (it == mCategories.end())
  {
    return nullptr;
  }

  IAppConfigCategory* rawInterfacePtr = it->get();
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-static-cast-downcast)
  return static_cast<AppConfigCategory*>(rawInterfacePtr);
}

nlohmann::json AppSettings::ToJson() const
{
  nlohmann::json root;
  root["version"] = mVersionString;
  for (auto &category: mCategories)
  {
    for (const auto &option: category->GetSettings())
    {
      root[category->GetName()][option->GetName()] = option->GetJson();
    }
  }

  return root;
}

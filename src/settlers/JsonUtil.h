#pragma once

#include <nlohmann/json.hpp>
#include <string>

class JsonUtil
{
 public:
  explicit JsonUtil();
  virtual ~JsonUtil();
  static bool readFile(const std::string& filePath, nlohmann::json& jsonData);
};

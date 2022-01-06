
#include "JsonUtil.h"

#include <spdlog/spdlog.h>

#include <fstream>

JsonUtil::JsonUtil()
{
}

JsonUtil::~JsonUtil()
{
}
bool JsonUtil::readFile(const std::string& filePath, nlohmann::json& jsonData)
{
  std::ifstream ifs;
  ifs.open(filePath, std::ifstream::in);
  if (ifs.fail())
  {
    SPDLOG_ERROR("failed to open file '{}'", filePath);
    return false;
  }
  bool success = false;
  try
  {
    ifs >> jsonData;
    success = true;
  }
  catch (nlohmann::json::parse_error& ex)
  {
    SPDLOG_ERROR("failed to parse file \"{}\": {}",filePath, ex.what());
  }
  ifs.close();
  if (success)
  {
    SPDLOG_INFO("successfully parsed json file \"{}\"", filePath);
  }
  return success;
}
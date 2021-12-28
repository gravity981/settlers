#include "settlers/FieldGenerator.h"

#include <fstream>
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <spdlog/spdlog.h>

FieldGenerator::FieldGenerator()
{
}

FieldGenerator::~FieldGenerator()
{
}

bool FieldGenerator::generateFromFile(const std::string& filePath)
{
  nlohmann::json jsonData;
  if(!readFile(filePath, jsonData))
  {
    return false;
  }
  if(!generateTiles(jsonData))
  {
    return false;
  }
  return true;
}

bool FieldGenerator::readFile(const std::string& filePath, nlohmann::json& jsonData)
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
    SPDLOG_ERROR("parse error at byte {}", ex.byte);
  }
  ifs.close();
  if (success)
  {
    SPDLOG_DEBUG("parsed data '{}'", jsonData.dump(4));
  }
  return success;
}
bool FieldGenerator::generateTiles(nlohmann::json jsonData)
{
  try
  {
    for (auto& obj : jsonData["tiles"].items())
    {
      //SPDLOG_INFO("tile q:{}, r:{}", obj["q"], obj["r"]);
      //SPDLOG_INFO("tile {}", obj.value()["q"]);
      SPDLOG_INFO("tile");
    }
  }
  catch(nlohmann::json::type_error& ex)
  {
    SPDLOG_ERROR("type error: {}", ex.what());
    return false;
  }
  return true;
}

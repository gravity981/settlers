#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include "settlers/TradingCenter.h"
#include "settlers/WorldGenerator.h"

TradingCenter::TradingCenter()
{
}

TradingCenter::~TradingCenter()
{
}
bool TradingCenter::initFromFile(const std::string& filePath)
{
  nlohmann::json jsonData;
  //todo move readFile function to some json helper class
  if(!WorldGenerator::readFile(filePath, jsonData))
  {
    return false;
  }
  try
  {
    for (auto& obj : jsonData)
    {
      auto typeStr = obj["type"].get<std::string>();
      //todo EBuyableObject from typeString
      Resource resource;
      for(auto& costsObj : obj["cost"])
      {
        auto resourceStr = costsObj["resource"].get<std::string>();
        //todo move ResourceFromString to Resource class
        auto resourceType = Harbour::ResourceFromString(resourceStr);
        auto amount = costsObj["amount"].get<int>();
        resource.set(resourceType, amount);
      }
      //todo use correct object
      m_buildingCosts.insert(std::make_pair(BUYABLEOBJECT_ROAD, resource));
    }
  }
  catch (nlohmann::json::type_error& ex)
  {
    SPDLOG_ERROR("failed to read building costs: {}", ex.what());
    return false;
  }
  return true;
}

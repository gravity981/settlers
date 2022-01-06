#include "settlers/TradingCenter.h"

#include <spdlog/spdlog.h>

#include <nlohmann/json.hpp>

#include "JsonUtil.h"

TradingCenter::TradingCenter()
{
}

TradingCenter::~TradingCenter()
{
}
bool TradingCenter::initFromFile(const std::string& filePath)
{
  nlohmann::json jsonData;
  if (!JsonUtil::readFile(filePath, jsonData))
  {
    return false;
  }
  try
  {
    for (auto& obj : jsonData)
    {
      auto objectStr = obj["object"].get<std::string>();
      auto object = strToBuyableObject(objectStr);
      Resource resource;
      for (auto& costsObj : obj["cost"])
      {
        auto resourceStr = costsObj["resource"].get<std::string>();
        auto resourceType = Resource::strToResource(resourceStr);
        auto amount = costsObj["amount"].get<int>();
        resource.set(resourceType, amount);
      }
      m_buildingCosts.insert(std::make_pair(object, resource));
    }
  }
  catch (nlohmann::json::type_error& ex)
  {
    SPDLOG_ERROR("failed to read building costs: {}", ex.what());
    return false;
  }
  return true;
}
TradingCenter::EBuyableObject TradingCenter::strToBuyableObject(const std::string& buyableObjectStr)
{
  if (buyableObjectStr == "road")
  {
    return BUYABLEOBJECT_ROAD;
  }
  if (buyableObjectStr == "village")
  {
    return BUYABLEOBJECT_VILLAGE;
  }
  if (buyableObjectStr == "city")
  {
    return BUYABLEOBJECT_CITY;
  }
  if (buyableObjectStr == "development")
  {
    return BUYABLEOBJECT_DEVELOPMENT;
  }
  SPDLOG_WARN("buyableObject \"{}\" is undefined", buyableObjectStr);
  return BUYABLEOBJECT_UNDEFINED;
}
Resource TradingCenter::getCostOf(EBuyableObject object) const
{
  if (m_buildingCosts.find(object) == m_buildingCosts.end())
  {
    SPDLOG_WARN("there is no cost registered for object \"{}\"", object);
    return Resource{};
  }
  else
  {
    return m_buildingCosts.at(object);
  }
}

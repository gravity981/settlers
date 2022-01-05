#pragma once

#include <map>
#include <string>
#include <vector>

#include "Resource.h"

class TradingCenter
{
 public:
  using EBuyableObject = enum EBuyableObject {
    BUYABLEOBJECT_ROAD,
    BUYABLEOBJECT_VILLAGE,
    BUYABLEOBJECT_CITY,
    BUYABLEOBJECT_DEVELOPMENT,
    BUYABLEOBJECT_UNDEFINED
  };

 private:
  std::map<EBuyableObject, Resource> m_buildingCosts;

 public:
  explicit TradingCenter();
  virtual ~TradingCenter();

  bool initFromFile(const std::string& filePath);
  Resource getCostOf(EBuyableObject object) const;

  static EBuyableObject strToBuyableObject(const std::string& buyableObjectStr);
};

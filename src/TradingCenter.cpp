#include <nlohmann/json.hpp>
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
  return WorldGenerator::readFile(filePath, jsonData);
}

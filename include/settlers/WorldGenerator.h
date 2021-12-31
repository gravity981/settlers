#pragma once

#include "Territory.h"
#include "Settlement.h"
#include "Road.h"

#include <nlohmann/json.hpp>
#include <string>

class WorldGenerator
{
 private:
  nlohmann::json m_jsonData;
  std::map<int, Territory> m_territoryMap;
  std::map<int, Settlement> m_settlementMap;
  std::map<int, Road> m_roadMap;
 public:
  explicit WorldGenerator();
  virtual ~WorldGenerator();

  bool generateFromFile(const std::string& filePath);

  const std::map<int, Territory>& getTerritories() const;
  const std::map<int, Settlement>& getSettlements() const;
  const std::map<int, Road>& getRoads() const;

 private:
  static bool readFile(const std::string& filePath, nlohmann::json& jsonData);
  bool createTerritories(nlohmann::json jsonData);
  bool calculateTileTypes();
  void calculateCoastTiles();
  bool calculateLandTiles();
  void createSettlementsAndRoads();
  void linkTerritoriesAndSettlements();
  void linkSettlementsAndRoads();
  void linkNeighborTerritories();
};

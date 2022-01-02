#pragma once

#include "Tile.h"
#include "Settlement.h"
#include "Road.h"

#include <nlohmann/json.hpp>
#include <string>

class WorldGenerator
{
 private:
  nlohmann::json m_jsonData;
  std::map<int, Tile> m_tileMap;
  std::map<int, Settlement> m_settlementMap;
  std::map<int, Road> m_roadMap;
 public:
  explicit WorldGenerator();
  virtual ~WorldGenerator();

  bool generateFromFile(const std::string& filePath);

  const std::map<int, Tile>& getTiles() const;
  const std::map<int, Settlement>& getSettlements() const;
  const std::map<int, Road>& getRoads() const;

 private:
  bool readFile(const std::string& filePath);
  bool createTiles();
  bool calculateTileTypes();
  void calculateCoastTiles();
  bool calculateLandTiles();
  void createSettlementsAndRoads();
  void linkTilesAndSettlements();
  void linkSettlementsAndRoads();
  void linkNeighborTiles();
};

#pragma once

#include "Tile.h"
#include "Corner.h"
#include "Road.h"

#include <nlohmann/json.hpp>
#include <string>

class WorldGenerator
{
 private:
  nlohmann::json m_jsonData;
  std::map<int, Tile> m_tileMap;
  std::map<int, Corner> m_cornerMap;
  std::map<int, Road> m_roadMap;
 public:
  explicit WorldGenerator();
  virtual ~WorldGenerator();

  bool generateFromFile(const std::string& filePath);

  const std::map<int, Tile>& getTiles() const;
  const std::map<int, Corner>& getCorners() const;
  const std::map<int, Road>& getRoads() const;

 private:
  bool readFile(const std::string& filePath);
  bool createTiles();
  bool calculateTileTypes();
  void calculateCoastTiles();
  bool calculateLandTiles();
  void createCornersAndRoads();
  void linkTilesAndCorners();
  void linkCornersAndRoads();
  void linkNeighborTiles();
};

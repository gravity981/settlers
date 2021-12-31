#pragma once

#include "Tile.h"
#include "Corner.h"
#include "Edge.h"

#include <nlohmann/json.hpp>
#include <string>

class WorldGenerator
{
 private:
  nlohmann::json m_jsonData;
  std::map<int, Tile> m_tileMap;
  std::map<int, Corner> m_cornerMap;
  std::map<int, Edge> m_edgeMap;
 public:
  explicit WorldGenerator();
  virtual ~WorldGenerator();

  bool generateFromFile(const std::string& filePath);

 private:
  static bool readFile(const std::string& filePath, nlohmann::json& jsonData);
  bool createTiles(nlohmann::json jsonData);
  bool calculateTileTypes();
  void calculateCoastTiles();
  bool calculateLandTiles();
  void createCornersAndEdges();
  void linkTilesAndCorners();
  void linkCornersAndEdges();
};

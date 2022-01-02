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

  const std::map<int, Tile>& getTiles() const;
  const std::map<int, Corner>& getCorners() const;
  const std::map<int, Edge>& getEdges() const;

 private:
  bool readFile(const std::string& filePath);
  bool createTiles();
  bool calculateTileTypes();
  void calculateCoastTiles();
  bool calculateLandTiles();
  void createCornersAndEdges();
  void linkTilesAndCorners();
  void linkCornersAndEdges();
  void linkNeighborTiles();
};

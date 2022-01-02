#pragma once

#include "Tile.h"
#include "Corner.h"
#include "Edge.h"
#include "Territory.h"

#include <nlohmann/json.hpp>
#include <string>

using TerritoryTypePool = std::vector<std::tuple<bool, int, int, Territory::EType>>;

class WorldGenerator
{
 private:
  unsigned long m_seed;
  nlohmann::json m_jsonData;
  std::map<int, Tile> m_tileMap;
  std::map<int, Corner> m_cornerMap;
  std::map<int, Edge> m_edgeMap;
  std::vector<Territory> m_territories;
 public:
  explicit WorldGenerator();
  virtual ~WorldGenerator();

  bool generateFromFile(const std::string& filePath, unsigned long seed);

  const std::map<int, Tile>& getTiles() const;
  const std::map<int, Corner>& getCorners() const;
  const std::map<int, Edge>& getEdges() const;

 private:
  bool readFile(const std::string& filePath);
  bool createTiles();
  void createCornersAndEdges();
  bool createTerritories();
  bool initTerritoryTypePool(TerritoryTypePool& territoryTypePool);
  bool createPredefinedTerritories(TerritoryTypePool& territoryTypePool);
  bool createCoastTerritories(TerritoryTypePool& territoryTypePool);
  bool createRandomTerritories(TerritoryTypePool& territoryTypePool);
  void createTerritory(Tile& tile, Territory::EType type);
  bool consumeTerritoryType(TerritoryTypePool& territoryTypePool, Territory::EType type);
  void linkTilesAndCorners();
  void linkCornersAndEdges();
  void linkNeighborTiles();
  void linkTilesAndEdges();
};

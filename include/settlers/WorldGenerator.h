#pragma once

#include "Tile.h"
#include "Corner.h"
#include "Edge.h"
#include "Sector.h"
#include "Territory.h"

#include <nlohmann/json.hpp>
#include <string>

using TerritoryPool = std::vector<std::tuple<bool, int, int, ITileObject::EType>>;
//using HarbourPool = std::vector<std::tuple<bool, int, int, int, Harbour::EEffect, Harbour::EResource>>;

class WorldGenerator
{
 private:
  unsigned long m_seed;
  nlohmann::json m_jsonData;
  std::map<int, Tile> m_tileMap;
  std::map<int, Corner> m_cornerMap;
  std::map<int, Edge> m_edgeMap;
  std::map<int, Sector> m_sectorMap;
  std::vector<Territory*> m_territories;
 public:
  explicit WorldGenerator();
  virtual ~WorldGenerator();

  bool generateFromFile(const std::string& filePath, unsigned long seed);

  const std::map<int, Tile>& getTiles() const;
  const std::map<int, Corner>& getCorners() const;
  const std::map<int, Edge>& getEdges() const;
  const std::map<int, Sector>& getSectors() const;

 private:
  bool readFile(const std::string& filePath);
  bool createTiles();
  void createCornersAndEdges();
  bool createTerritories();
  void createSectors();
  bool initTerritoryTypePool(TerritoryPool& territoryTypePool);
  bool createPredefinedTerritories(TerritoryPool& territoryTypePool);
  bool createCoastTerritories(TerritoryPool& territoryTypePool);
  bool createRandomTerritories(TerritoryPool& territoryTypePool);
  void createTerritory(Tile& tile, Territory::EType type);
  bool consumeTerritoryType(TerritoryPool& territoryTypePool, Territory::EType type);
  void linkTilesAndCorners();
  void linkCornersAndEdges();
  void linkTilesAndEdges();
  std::vector<std::reference_wrapper<Tile>> getNeighborTiles(const Tile& tile);
  bool createHarbours();
};

#pragma once

#include <nlohmann/json.hpp>
#include <random>
#include <string>

#include "Corner.h"
#include "Edge.h"
#include "Harbour.h"
#include "Sector.h"
#include "Territory.h"
#include "Tile.h"

using TerritoryPool = std::vector<std::tuple<bool, int, int, ITileObject::EType>>;
using HarbourPool = std::vector<std::tuple<bool, int, int, int, Harbour::EEffect, Harbour::EResource>>;

class WorldGenerator
{
 private:
  std::default_random_engine m_randomEngine{};
  nlohmann::json m_jsonData;
  std::map<int, Tile> m_tileMap;
  std::map<int, Corner> m_cornerMap;
  std::map<int, Edge> m_edgeMap;
  std::map<int, Sector> m_sectorMap;
  std::vector<Territory*> m_territories;
  std::vector<Harbour*> m_harbours;

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
  void createSectors();
  bool createTerritories();
  bool createHarbours();
  bool initHarbourPool(HarbourPool& harbourPool);
  bool createPredefinedHarbours(HarbourPool& harbourPool);
  bool createRandomHarbours(HarbourPool& harbourPool);
  void createHarbour(Sector& sector, Harbour::EEffect effect, Harbour::EResource resource);
  bool initTerritoryPool(TerritoryPool& territoryPool);
  bool createPredefinedTerritories(TerritoryPool& territoryPool);
  bool createCoastTerritories(TerritoryPool& territoryPool);
  bool createRandomTerritories(TerritoryPool& territoryPool);
  void createTerritory(Tile& tile, Territory::EType type);
  bool consumeTerritoryType(TerritoryPool& territoryTypePool, Territory::EType type);
  void linkTilesAndCorners();
  void linkCornersAndEdges();
  void linkTilesAndEdges();
  std::vector<std::reference_wrapper<Tile>> getNeighborTiles(const Tile& tile);
};

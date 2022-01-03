#include "settlers/WorldGenerator.h"

#include <spdlog/spdlog.h>

#include <cstdlib>
#include <fstream>
#include <random>

#include "settlers/Territory.h"

WorldGenerator::WorldGenerator()
{
}

WorldGenerator::~WorldGenerator()
{
}

bool WorldGenerator::generateFromFile(const std::string& filePath, unsigned long seed)
{
  m_seed = seed;
  m_jsonData.clear();
  m_tileMap.clear();
  m_cornerMap.clear();
  m_edgeMap.clear();
  // read file
  if (!readFile(filePath))
  {
    return false;
  }

  // world related stuff
  if (!createTiles())
  {
    return false;
  }
  createCornersAndEdges();
  linkTilesAndCorners();
  linkTilesAndEdges();
  linkCornersAndEdges();
  createSectors();

  // game related stuff
  if (!createTerritories())
  {
    return false;
  }
  return true;
}

bool WorldGenerator::readFile(const std::string& filePath)
{
  std::ifstream ifs;
  ifs.open(filePath, std::ifstream::in);
  if (ifs.fail())
  {
    SPDLOG_ERROR("failed to open file '{}'", filePath);
    return false;
  }
  bool success = false;
  try
  {
    ifs >> m_jsonData;
    success = true;
  }
  catch (nlohmann::json::parse_error& ex)
  {
    SPDLOG_ERROR("parse error at byte {}", ex.byte);
  }
  ifs.close();
  if (success)
  {
    SPDLOG_INFO("parsed json file '{}'", filePath);
  }
  return success;
}

bool WorldGenerator::createTiles()
{
  bool success = true;
  try
  {
    for (auto& obj : m_jsonData["tiles"])
    {
      auto q = obj["q"].get<int>();
      auto r = obj["r"].get<int>();
      auto type = obj.value("type", "");
      Tile tile{ q, r };
      if (m_tileMap.find(tile.id()) == m_tileMap.end())
      {
        m_tileMap.insert(std::make_pair(tile.id(), tile));
      }
      else
      {
        SPDLOG_ERROR("duplicated tiles detected q: {}, r: {}, id: {}", q, r, tile.id());
        success = false;
      }
    }
  }
  catch (nlohmann::json::type_error& ex)
  {
    SPDLOG_ERROR("{}", ex.what());
    return false;
  }
  if (success)
  {
    SPDLOG_INFO("generated tiles");
  }
  return success;
}

bool WorldGenerator::createTerritories()
{
  std::vector<std::tuple<bool, int, int, Territory::EType>> territoryTypePool;
  if (!initTerritoryTypePool(territoryTypePool))
  {
    return false;
  }
  if (!createPredefinedTerritories(territoryTypePool))
  {
    return false;
  }
  if (!createCoastTerritories(territoryTypePool))
  {
    return false;
  }
  if (!createRandomTerritories(territoryTypePool))
  {
    return false;
  }
  if (!territoryTypePool.empty())
  {
    SPDLOG_ERROR("there are {} remaining territories to be assigned", territoryTypePool.size());
    return false;
  }
  return true;
}

bool WorldGenerator::createCoastTerritories(TerritoryTypePool& territoryTypePool)
{
  for (auto& [id, tile] : m_tileMap)
  {
    // all tiles which are at the border of the grid are considered coast.
    if (getNeighborTiles(tile).size() != 6 && tile.getTileObject() == nullptr)
    {
      if (consumeTerritoryType(territoryTypePool, Territory::TYPE_COAST))
      {
        createTerritory(tile, Territory::TYPE_COAST);
      }
      else
      {
        SPDLOG_ERROR("failed to create coast territories");
        return false;
      }
    }
  }
  return true;
}

bool WorldGenerator::createRandomTerritories(TerritoryTypePool& territoryTypePool)
{
  std::default_random_engine randomEngine(m_seed);
  for (auto& [id, tile] : m_tileMap)
  {
    if (tile.getTileObject() == nullptr)
    {
      if (territoryTypePool.empty())
      {
        SPDLOG_ERROR("territory type pool starved");
        return false;
      }
      std::uniform_int_distribution<size_t> uniform_dist(0, territoryTypePool.size() - 1);
      auto randomIndex = uniform_dist(randomEngine);
      auto entry = territoryTypePool.at(randomIndex);
      createTerritory(tile, std::get<3>(entry));
      // consume from pool
      territoryTypePool.erase(std::next(territoryTypePool.begin(), static_cast<long>(randomIndex)));
    }
  }
  return true;
}

bool WorldGenerator::createPredefinedTerritories(TerritoryTypePool& territoryTypePool)
{
  for (auto it = territoryTypePool.begin(); it != territoryTypePool.end();)
  {
    auto entry = *it;
    Tile tmpTile = Tile{ std::get<1>(entry), std::get<2>(entry) };
    if (!std::get<0>(entry))
    {
      // assign predefined territory type to tile and consume from pool
      if (m_tileMap.find(tmpTile.id()) != m_tileMap.end())
      {
        createTerritory(m_tileMap.at(tmpTile.id()), std::get<3>(entry));
      }
      else
      {
        SPDLOG_ERROR("tile for predefined territory q: {}, r: {} does not exist", tmpTile.q(), tmpTile.r());
        return false;
      }
      it = territoryTypePool.erase(it);
    }
    else
    {
      it++;
    }
  }
  return true;
}

bool WorldGenerator::consumeTerritoryType(TerritoryTypePool& territoryTypePool, Territory::EType type)
{
  for (auto it = territoryTypePool.begin(); it != territoryTypePool.end(); it++)
  {
    if (std::get<3>(*it) == type)
    {
      territoryTypePool.erase(it);
      return true;
    }
  }
  SPDLOG_ERROR("no more {}-type territories in pool", type);
  return false;
}

void WorldGenerator::createTerritory(Tile& tile, Territory::EType type)
{
  Territory territory{ tile };
  territory.setType(type);
  m_territories.emplace_back(territory);
  tile.setTileObject(&m_territories.back());
}

bool WorldGenerator::initTerritoryTypePool(TerritoryTypePool& territoryTypePool)
{
  try
  {
    for (const auto& obj : m_jsonData["territories"])
    {
      auto typeStr = obj["type"].get<std::string>();
      auto type = Territory::typeFromString(typeStr);
      if (type == Territory::TYPE_UNDEFINED)
      {
        SPDLOG_WARN("type \"{}\" is undefined, skip", typeStr);
      }
      else
      {
        auto amount = obj["amount"].get<int>();
        if (!obj.contains("pos"))
        {
          for (int i = 0; i < amount; i++)
          {
            territoryTypePool.push_back(std::make_tuple(true, 0, 0, type));
          }
        }
        else
        {
          territoryTypePool.push_back(std::make_tuple(false, obj["pos"]["q"].get<int>(), obj["pos"]["r"].get<int>(), type));
        }
      }
    }
  }
  catch (nlohmann::json::type_error& ex)
  {
    SPDLOG_ERROR("{}", ex.what());
    return false;
  }
  SPDLOG_INFO("territoryTypePool size : {}", territoryTypePool.size());
  return true;
}

void WorldGenerator::createCornersAndEdges()
{
  // loop through all tiles
  for (auto& [id, tile] : m_tileMap)
  {
    auto neighbors = getNeighborTiles(tile);
    // loop through each neighbors
    std::vector<Corner> overlappingCorners;
    for (auto& neighbor : neighbors)
    {
      const auto& neighborCorners = neighbor.get().getAllPossibleCorners();
      auto possibleCorners = tile.getAllPossibleCorners();
      overlappingCorners = Corner::getOverlappingCorners(possibleCorners, neighborCorners);
      if (overlappingCorners.size() != 2)
      {
        SPDLOG_WARN("unexpected number of overlapping corners between two tiles: {}", overlappingCorners.size());
      }
      else
      {
        // create corners
        for (auto& overlappingCorner : overlappingCorners)
        {
          auto cornerId = overlappingCorner.id();
          // create corner if not exist
          if (m_cornerMap.find(cornerId) == m_cornerMap.end())
          {
            m_cornerMap.insert(std::make_pair(cornerId, Corner{ overlappingCorner.q(), overlappingCorner.r() }));
          }
        }

        // todo edge creation could be done in a separate step to reduce function length
        // create edge between neighbors if not exists already
        auto edgeId = Edge::id(overlappingCorners);
        if (m_edgeMap.find(edgeId) == m_edgeMap.end())
        {
          Edge edge = Edge{
            overlappingCorners[0].q(), overlappingCorners[0].r(), overlappingCorners[1].q(), overlappingCorners[1].r()
          };
          m_edgeMap.insert(std::make_pair(edgeId, edge));

          // todo linking could be done in a separate step to make things easier to read
          m_edgeMap.at(edgeId).addCorner(m_cornerMap.at(overlappingCorners[0].id()));
          m_edgeMap.at(edgeId).addCorner(m_cornerMap.at(overlappingCorners[1].id()));
        }
      }
    }
  }
}

void WorldGenerator::linkTilesAndCorners()
{
  for (auto& [id, tile] : m_tileMap)
  {
    for (const auto& corner : tile.getAllPossibleCorners())
    {
      if (m_cornerMap.find(corner.id()) != m_cornerMap.end())
      {
        // link tiles and corners
        tile.addCorner(m_cornerMap.at(corner.id()));
        // link corners and tiles
        m_cornerMap.at(corner.id()).addTile(tile);
      }
    }
  }
}

void WorldGenerator::linkCornersAndEdges()
{
  // edges know their corners already after creation, so simply iterate over corners of edges
  for (auto& [edgeId, edge] : m_edgeMap)
  {
    for (auto& corner : edge.getCorners())
    {
      corner.get().addEdge(edge);
    }
  }
}

void WorldGenerator::linkTilesAndEdges()
{
  for (auto& [id, tile] : m_tileMap)
  {
    auto possibleEdges = tile.getAllPossibleEdges();
    for (const auto& edge : possibleEdges)
    {
      if (m_edgeMap.find(edge.id()) != m_edgeMap.end())
      {
        // link tiles and edges
        auto& existingEdge = m_edgeMap.at(edge.id());
        tile.addEdge(existingEdge);
        // link edges and tiles
        existingEdge.addTile(tile);
      }
    }
  }
}

const std::map<int, Tile>& WorldGenerator::getTiles() const
{
  return m_tileMap;
}

const std::map<int, Corner>& WorldGenerator::getCorners() const
{
  return m_cornerMap;
}

const std::map<int, Edge>& WorldGenerator::getEdges() const
{
  return m_edgeMap;
}
std::vector<std::reference_wrapper<Tile>> WorldGenerator::getNeighborTiles(const Tile& tile)
{
  std::vector<std::reference_wrapper<Tile>> neighbors;
  for (auto& possibleNeighbor : tile.getAllPossibleNeighbors())
  {
    if (m_tileMap.find(possibleNeighbor.id()) != m_tileMap.end())
    {
      neighbors.emplace_back(m_tileMap.at(possibleNeighbor.id()));
    }
  }
  return neighbors;
}
void WorldGenerator::createSectors()
{
  for (auto& [id, tile] : m_tileMap)
  {
    // there is a sector for each existing edge
    for (auto& edge : tile.getEdges())
    {
      auto corners = edge.get().getCorners();
      if (corners.size() != 2)
      {
        SPDLOG_WARN("edge with unexpected corner count, skip");
      }
      else
      {
        //create sector and link with tile and corners
        Sector sector{
            static_cast<double>(tile.q()),
            static_cast<double>(tile.r()),
            corners[0].get().q(),
            corners[0].get().r(),
            corners[1].get().q(),
            corners[1].get().r(),
            tile};
        sector.addCorner(corners[0].get());
        sector.addCorner(corners[1].get());
        m_sectorMap.insert(std::make_pair(sector.id(), sector));

        //link corners with sector
        auto& realSector = m_sectorMap.at(sector.id());
        corners[0].get().addSector(realSector);
        corners[1].get().addSector(realSector);

        //link tile with sector
        tile.addSector(realSector);
      }
    }
  }
}
const std::map<int, Sector>& WorldGenerator::getSectors() const
{
  return m_sectorMap;
}

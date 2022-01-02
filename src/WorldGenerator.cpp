#include "settlers/WorldGenerator.h"
#include "settlers/Territory.h"

#include <cstdlib>
#include <fstream>
#include <spdlog/spdlog.h>

#include <random>

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
  linkNeighborTiles(); //must happen before creating corners and edges
  createCornersAndEdges();
  linkTilesAndCorners();
  linkCornersAndEdges();

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
  if(!initTerritoryTypePool(territoryTypePool))
  {
    return false;
  }
  if(!createPredefinedTerritories(territoryTypePool))
  {
    return false;
  }
  if(!createCoastTerritories(territoryTypePool))
  {
    return false;
  }
  if(!createRandomTerritories(territoryTypePool))
  {
    return false;
  }
  if(!territoryTypePool.empty())
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
    if (!tile.allNeighborsExist() && tile.getTileObject() == nullptr)
    {
      if(consumeTerritoryType(territoryTypePool, Territory::TYPE_COAST))
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
      //consume from pool
      territoryTypePool.erase(std::next(territoryTypePool.begin(), static_cast<long>(randomIndex)));
    }
  }
  return true;
}

bool WorldGenerator::createPredefinedTerritories(
    TerritoryTypePool& territoryTypePool)
{
  for(auto it = territoryTypePool.begin(); it != territoryTypePool.end(); )
  {
    auto entry = *it;
    Tile tmpTile = Tile{std::get<1>(entry), std::get<2>(entry)};
    if(!std::get<0>(entry))
    {
      //assign predefined territory type to tile and consume from pool
      if(m_tileMap.find(tmpTile.id()) != m_tileMap.end())
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
  for(auto it = territoryTypePool.begin(); it != territoryTypePool.end(); it++)
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
    Territory territory{tile};
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
      if(type == Territory::TYPE_UNDEFINED)
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
    auto& neighbors = tile.getNeighbors();
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

      // create edge between neighbors if not exists already
      auto edgeId = Edge::id(overlappingCorners);
      bool isNewEdge = false;
      if (m_edgeMap.find(edgeId) == m_edgeMap.end())
      {
        isNewEdge = true;
        Edge edge{};
        edge.addTile(tile);
        edge.addTile(neighbor);
        m_edgeMap.insert(std::make_pair(edgeId, edge));
      }

      // loop through overlapping corners
      for (auto& overlappingCorner : overlappingCorners)
      {
        auto cornerId = overlappingCorner.id();
        // create corner if not exist
        if (m_cornerMap.find(cornerId) == m_cornerMap.end())
        {
          m_cornerMap.insert(std::make_pair(cornerId, Corner{overlappingCorner.q(), overlappingCorner.r()}));
        }
        // add corner to edge
        if (isNewEdge)
        {
          m_edgeMap.at(edgeId).addCorner(m_cornerMap.at(cornerId));
        }
      }
    }
  }
}

void WorldGenerator::linkNeighborTiles()
{
  for (auto& [id, tile] : m_tileMap)
  {
    auto possibleNeighbors = tile.getAllPossibleNeighbors();
    for (const auto& possibleNeighbor : possibleNeighbors)
    {
      if (m_tileMap.find(possibleNeighbor.id()) != m_tileMap.end())
      {
        tile.addNeighbor(m_tileMap.at(possibleNeighbor.id()));
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
        tile.addCorner(m_cornerMap.at(corner.id()));
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

#include "settlers/WorldGenerator.h"

#include <cstdlib>
#include <fstream>
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <spdlog/spdlog.h>

#include <random>

WorldGenerator::WorldGenerator()
{
}

WorldGenerator::~WorldGenerator()
{
}

bool WorldGenerator::generateFromFile(const std::string& filePath)
{
  m_jsonData.clear();
  m_tileMap.clear();
  m_settlementMap.clear();
  m_roadMap.clear();
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
  linkNeighborTiles(); //must happen before creating settlements and roads
  createSettlementsAndRoads();
  linkTilesAndSettlements();
  linkSettlementsAndRoads();

  // game related stuff
  if (!calculateTileTypes())
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
      tile.setType(Tile::typeFromString(type)); //todo this should be moved to another function for easier regeneration
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

bool WorldGenerator::calculateTileTypes()
{
  calculateCoastTiles();
  return calculateLandTiles();
}

void WorldGenerator::calculateCoastTiles()
{
  for (auto& [id, tile] : m_tileMap)
  {
    // all tiles which are at the border of the grid are considered coast.
    if (!tile.allNeighborsExist() && tile.getType() == Tile::TYPE_UNDEFINED)
    {
      tile.setType(Tile::TYPE_COAST);
    }
  }
}

bool WorldGenerator::calculateLandTiles()
{
  // create typePool;
  std::vector<Tile::EType> typePool;
  try
  {
    for (const auto& obj : m_jsonData["pool"])
    {
      auto type = Tile::typeFromString(obj["type"].get<std::string>());
      auto amount = obj["amount"].get<int>();
      for (int i = 0; i < amount; i++)
      {
        typePool.push_back(type);
      }
    }
  }
  catch (nlohmann::json::type_error& ex)
  {
    SPDLOG_ERROR("{}", ex.what());
    return false;
  }
  SPDLOG_INFO("raw typePool size: {}", typePool.size());

  // remove already assigned from tilePool
  for (auto& [id, tile] : m_tileMap)
  {
    auto it = std::find(typePool.begin(), typePool.end(), tile.getType());
    if (it != typePool.end())
    {
      typePool.erase(it);
    }
  }
  SPDLOG_INFO("typePool size after cleaning: {}", typePool.size());

  // randomly assign tile types to still undefined tiles
  bool typePoolStarved = false;
  // Seed with a real random value, if available
  std::random_device randomDevice;

  // Choose a random mean between 1 and 6
  std::default_random_engine randomEngine(randomDevice());
  for (auto& [id, tile] : m_tileMap)
  {
    if (tile.getType() == Tile::TYPE_UNDEFINED)
    {
      if (typePool.empty())
      {
        typePoolStarved = true;
        break;
      }
      std::uniform_int_distribution<size_t> uniform_dist(0, typePool.size());
      auto randomIndex = uniform_dist(randomEngine);
      tile.setType(typePool.at(randomIndex));
      typePool.erase(std::next(typePool.begin(), static_cast<long>(randomIndex)));
    }
  }
  SPDLOG_INFO("typePool size after assigning remaining tiles: {}", typePool.size());
  if (!typePool.empty() || typePoolStarved)
  {
    SPDLOG_ERROR("mismatch between amount of existing tiles and typePool size");
    return false;
  }
  return true;
}

void WorldGenerator::createSettlementsAndRoads()
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
      if (m_roadMap.find(edgeId) == m_roadMap.end())
      {
        isNewEdge = true;
        m_roadMap.insert(std::make_pair(edgeId, Road{}));
      }

      // loop through overlapping corners
      for (auto& overlappingCorner : overlappingCorners)
      {
        auto cornerId = overlappingCorner.id();
        // create corner if not exist
        if (m_settlementMap.find(cornerId) == m_settlementMap.end())
        {
          m_settlementMap.insert(std::make_pair(cornerId, Settlement{overlappingCorner.q(), overlappingCorner.r()}));
        }
        // add corner to edge
        if (isNewEdge)
        {
          m_roadMap.at(edgeId).addCorner(m_settlementMap.at(cornerId));
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

void WorldGenerator::linkTilesAndSettlements()
{
  for (auto& [id, tile] : m_tileMap)
  {
    for (const auto& corner : tile.getAllPossibleCorners())
    {
      if (m_settlementMap.find(corner.id()) != m_settlementMap.end())
      {
        tile.addCorner(m_settlementMap.at(corner.id()));
        m_settlementMap.at(corner.id()).addTile(tile);
      }
    }
  }
}

void WorldGenerator::linkSettlementsAndRoads()
{
  // edges know their corners already after creation, so simply iterate over corners of edges
  for (auto& [edgeId, road] : m_roadMap)
  {
    for (auto& corner : road.getCorners())
    {
      corner.get().addEdge(road);
    }
  }
}

const std::map<int, Tile>& WorldGenerator::getTiles() const
{
  return m_tileMap;
}

const std::map<int, Settlement>& WorldGenerator::getSettlements() const
{
  return m_settlementMap;
}

const std::map<int, Road>& WorldGenerator::getRoads() const
{
  return m_roadMap;
}

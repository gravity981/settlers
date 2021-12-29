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
  if (!readFile(filePath, m_jsonData))
  {
    return false;
  }
  if (!generateTiles(m_jsonData))
  {
    return false;
  }
  generateCornersAndEdges();
  if (!calculateTileTypes())
  {
    return false;
  }
  return true;
}

bool WorldGenerator::readFile(const std::string& filePath, nlohmann::json& jsonData)
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
    ifs >> jsonData;
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
bool WorldGenerator::generateTiles(nlohmann::json jsonData)
{
  bool success = true;
  try
  {
    for (auto& obj : jsonData["tiles"])
    {
      auto q = obj["q"].get<int>();
      auto r = obj["r"].get<int>();
      auto type = obj.value("type", "");
      Tile tile{ q, r };
      tile.setType(Tile::typeFromString(type));
      if (m_tileMap.find(tile.id()) == m_tileMap.end())
      {
        m_tileMap.insert(std::make_pair(tile.id(), tile));
      }
      else
      {
        SPDLOG_ERROR("duplicated tile detected q: {}, r: {}, id: {}", q, r, tile.id());
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
    bool allNeighborsExist = true;
    for (const auto& neighborTile : tile.getAllPossibleNeighbors())
    {
      if (m_tileMap.find(neighborTile.id()) == m_tileMap.end())
      {
        allNeighborsExist = false;
        break;
      }
    }
    // all tiles which are at the border of the grid are considered coast.
    if (!allNeighborsExist && tile.getType() == Tile::TYPE_UNDEFINED)
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
void WorldGenerator::generateCornersAndEdges()
{
  for (auto& [id, tile] : m_tileMap)
  {
    // todo
    // get existing neighbors
    // loop through each neighbor
    // check if corners and edge exist for that neighbor
    // create corners and edge if not exist
    // link elements together
  }
}
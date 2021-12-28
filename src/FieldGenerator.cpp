#include "settlers/FieldGenerator.h"

#include <fstream>
#include <cstdlib>
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <spdlog/spdlog.h>

FieldGenerator::FieldGenerator()
{
}

FieldGenerator::~FieldGenerator()
{
}

bool FieldGenerator::generateFromFile(const std::string& filePath)
{
  m_jsonData.clear();
  if(!readFile(filePath, m_jsonData))
  {
    return false;
  }
  if(!generateTiles(m_jsonData))
  {
    return false;
  }
  if(!calculateTileTypes())
  {
    return false;
  }
  return true;
}

bool FieldGenerator::readFile(const std::string& filePath, nlohmann::json& jsonData)
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
bool FieldGenerator::generateTiles(nlohmann::json jsonData)
{
  bool success = true;
  try
  {
    for (auto& obj : jsonData["tiles"])
    {
      auto q= obj["q"].get<int>();
      auto r = obj["r"].get<int>();
      auto type = obj.value("type", "");
      Tile tile{q, r};
      tile.setType(Tile::typeFromString(type));
      if(m_tileMap.find(tile.id()) == m_tileMap.end())
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
  catch(nlohmann::json::type_error& ex)
  {
    SPDLOG_ERROR("{}", ex.what());
    return false;
  }
  if(success)
  {
    SPDLOG_INFO("generated tiles");
  }
  return success;
}
bool FieldGenerator::calculateTileTypes()
{
  calculateCoastTiles();
  return calculateLandTiles();
}
void FieldGenerator::calculateCoastTiles()
{
  for(auto& [id, tile] : m_tileMap)
  {
    bool allNeighborsExist = true;
    for(const auto& neighborTile : tile.getNeighbors())
    {
      if(m_tileMap.find(neighborTile.id()) == m_tileMap.end())
      {
        allNeighborsExist = false;
        break;
      }
    }
    //all tiles which are at the border of the grid are considered coast.
    if(!allNeighborsExist && tile.getType() == Tile::TYPE_UNDEFINED)
    {
      tile.setType(Tile::TYPE_COAST);
    }
  }
}
bool FieldGenerator::calculateLandTiles()
{
  //create typePool;
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
  catch(nlohmann::json::type_error& ex)
  {
    SPDLOG_ERROR("{}", ex.what());
    return false;
  }
  SPDLOG_INFO("raw typePool size: {}", typePool.size());

  //remove already assigned from tilePool
  for(auto& [id, tile] : m_tileMap)
  {
    auto it = std::find(typePool.begin(), typePool.end(), tile.getType());
    if(it != typePool.end())
    {
      typePool.erase(it);
    }
  }
  SPDLOG_INFO("typePool size after cleaning: {}", typePool.size());

  //randomly assign tile types to still undefined tiles
  bool typePoolStarved = false;
  for(auto& [id, tile] : m_tileMap)
  {
    if(tile.getType() == Tile::TYPE_UNDEFINED)
    {
      if(typePool.empty())
      {
        typePoolStarved = true;
        break;
      }
      auto randomIndex = std::rand() % typePool.size();
      tile.setType(typePool.at(randomIndex));
      typePool.erase(typePool.begin() + randomIndex);
    }
  }
  SPDLOG_INFO("typePool size after assigning remaining tiles: {}", typePool.size());
  if(!typePool.empty() || typePoolStarved)
  {
    SPDLOG_ERROR("mismatch between amount of existing tiles and typePool size");
    return false;
  }
  return true;
}

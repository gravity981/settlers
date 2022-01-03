#include "settlers/WorldGenerator.h"

#include <spdlog/spdlog.h>

#include <cstdlib>
#include <fstream>


#include "settlers/Territory.h"

WorldGenerator::WorldGenerator()
{
}

WorldGenerator::~WorldGenerator()
{
}

bool WorldGenerator::generateFromFile(const std::string& filePath, unsigned long seed)
{
  m_randomEngine.seed(seed);
  m_jsonData.clear();
  m_tileMap.clear();
  m_cornerMap.clear();
  m_edgeMap.clear();
  for (auto* territory : m_territories)
  {
    delete territory;
  }
  m_territories.clear();
  for (auto* harbour : m_harbours)
  {
    delete harbour;
  }
  m_harbours.clear();
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
  if (!createHarbours())
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
    SPDLOG_INFO("generated {} tiles", m_tileMap.size());
  }
  return success;
}

bool WorldGenerator::createTerritories()
{
  TerritoryPool territoryPool;
  if (!initTerritoryPool(territoryPool))
  {
    return false;
  }
  auto territoryPoolCount = territoryPool.size();
  if (!createPredefinedTerritories(territoryPool))
  {
    return false;
  }
  if (!createCoastTerritories(territoryPool))
  {
    return false;
  }
  if (!createRandomTerritories(territoryPool))
  {
    return false;
  }
  if (!territoryPool.empty())
  {
    SPDLOG_ERROR("{} territories could not be placed", territoryPool.size());
    return false;
  }
  SPDLOG_INFO("placed {} territories", territoryPoolCount);
  return true;
}

bool WorldGenerator::createCoastTerritories(TerritoryPool& territoryPool)
{
  for (auto& [id, tile] : m_tileMap)
  {
    // all tiles which are at the border of the grid are considered coast.
    if (getNeighborTiles(tile).size() != 6 && tile.getTileObject() == nullptr)
    {
      if (consumeTerritoryType(territoryPool, Territory::TYPE_COAST))
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

bool WorldGenerator::createRandomTerritories(TerritoryPool& territoryPool)
{

  for (auto& [id, tile] : m_tileMap)
  {
    if (tile.getTileObject() == nullptr)
    {
      if (territoryPool.empty())
      {
        SPDLOG_ERROR("territory type pool starved");
        return false;
      }
      std::uniform_int_distribution<size_t> uniform_dist(0, territoryPool.size() - 1);
      auto randomIndex = uniform_dist(m_randomEngine);
      auto entry = territoryPool.at(randomIndex);
      createTerritory(tile, std::get<3>(entry));
      // consume from pool
      territoryPool.erase(std::next(territoryPool.begin(), static_cast<long>(randomIndex)));
    }
  }
  return true;
}

bool WorldGenerator::createPredefinedTerritories(TerritoryPool& territoryPool)
{
  for (auto it = territoryPool.begin(); it != territoryPool.end();)
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
      it = territoryPool.erase(it);
    }
    else
    {
      it++;
    }
  }
  return true;
}

bool WorldGenerator::consumeTerritoryType(TerritoryPool& territoryTypePool, Territory::EType type)
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
  auto territory = new Territory{ tile };
  //todo set type in constructor
  territory->setType(type);
  m_territories.emplace_back(territory);
  tile.setTileObject(m_territories.back());
}

bool WorldGenerator::initTerritoryPool(TerritoryPool& territoryPool)
{
  try
  {
    for (const auto& obj : m_jsonData["territories"])
    {
      auto typeStr = obj["type"].get<std::string>();
      auto type = Territory::typeFromString(typeStr);
      if (type == Territory::TYPE_UNDEFINED)
      {
        SPDLOG_WARN("skip territory because of undefined type \"{}\"", typeStr);
      }
      else
      {
        auto amount = obj["amount"].get<int>();
        if (!obj.contains("pos"))
        {
          for (int i = 0; i < amount; i++)
          {
            territoryPool.push_back(std::make_tuple(true, 0, 0, type));
          }
        }
        else
        {
          territoryPool.push_back(std::make_tuple(false, obj["pos"]["q"].get<int>(), obj["pos"]["r"].get<int>(), type));
        }
      }
    }
  }
  catch (nlohmann::json::type_error& ex)
  {
    SPDLOG_ERROR("could not init territoryPool: {}", ex.what());
    return false;
  }
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
  SPDLOG_INFO("generated {} corners", m_cornerMap.size());
  SPDLOG_INFO("generated {} edges", m_edgeMap.size());
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
        // create sector and link with tile and corners
        Sector sector{ static_cast<double>(tile.q()),
                       static_cast<double>(tile.r()),
                       corners[0].get().q(),
                       corners[0].get().r(),
                       corners[1].get().q(),
                       corners[1].get().r(),
                       tile,
                       edge };
        m_sectorMap.insert(std::make_pair(sector.id(), sector));

        // link corners with sector
        auto& realSector = m_sectorMap.at(sector.id());
        corners[0].get().addSector(realSector);
        corners[1].get().addSector(realSector);

        // link tile with sector
        tile.addSector(realSector);
      }
    }
  }
  SPDLOG_INFO("generated {} sectors", m_sectorMap.size());
}
const std::map<int, Sector>& WorldGenerator::getSectors() const
{
  return m_sectorMap;
}
bool WorldGenerator::createHarbours()
{
  HarbourPool harbourPool;
  if(!initHarbourPool(harbourPool))
  {
    return false;
  }
  auto harbourPoolCount = harbourPool.size();
  if(!createPredefinedHarbours(harbourPool))
  {
    return false;
  }
  if(!createRandomHarbours(harbourPool))
  {
    return false;
  }
  if (!harbourPool.empty())
  {
    SPDLOG_ERROR("{} harbours could not be placed", harbourPool.size());
    return false;
  }
  SPDLOG_INFO("placed {} harbours", harbourPoolCount);
  return true;
}
bool WorldGenerator::initHarbourPool(HarbourPool& harbourPool)
{
  try
  {
    for (const auto& obj : m_jsonData["harbours"])
    {
      auto effectStr = obj["effect"].get<std::string>();
      auto resourceStr = obj["resource"].get<std::string>();
      auto effect = Harbour::EffectFromString(effectStr);
      auto resource = Harbour::ResourceFromString(resourceStr);
      bool wellDefined = true;
      if (effect == Harbour::EFFECT_UNDEFINED)
      {
        SPDLOG_WARN("skip harbour because of undefined effect \"{}\"", effectStr);
        wellDefined = false;
      }
      if (resource == Harbour::RESOURCE_UNDEFINED)
      {
        SPDLOG_WARN("skip harbour because of undefined resource \"{}\"", resourceStr);
        wellDefined = false;
      }
      if (wellDefined)
      {
        auto amount = obj["amount"].get<int>();
        if (!obj.contains("pos"))
        {
          for (int i = 0; i < amount; i++)
          {
            harbourPool.push_back(std::make_tuple(true, 0, 0, 0, effect, resource));
          }
        }
        else
        {
          auto q = obj["pos"]["q"].get<int>();
          auto r = obj["pos"]["r"].get<int>();
          auto nr = obj["pos"]["nr"].get<int>();
          harbourPool.push_back(std::make_tuple(false, q, r, nr, effect, resource));
        }
      }
    }
  }
  catch (nlohmann::json::type_error& ex)
  {
    SPDLOG_ERROR("could not init harbourPool: {}", ex.what());
    return false;
  }
  return true;
}
bool WorldGenerator::createPredefinedHarbours(HarbourPool& harbourPool)
{
  for (auto it = harbourPool.begin(); it != harbourPool.end();)
  {
    auto entry = *it;
    auto isRandomHarbour = std::get<0>(entry);
    auto q = std::get<1>(entry);
    auto r = std::get<2>(entry);
    auto nr = std::get<3>(entry);
    auto effect = std::get<4>(entry);
    auto resource = std::get<5>(entry);
    if (!isRandomHarbour)
    {
      auto sectorId = Sector::id(q, r, nr);
      if (m_sectorMap.find(sectorId) != m_sectorMap.end())
      {
        auto sector = m_sectorMap.at(sectorId);
        if(sector.getTile().getTileObject()->type() != ITileObject::TYPE_COAST ||
            sector.getOppositeTile()->getTileObject()->type() == ITileObject::TYPE_COAST)
        {
          SPDLOG_WARN("predefined harbour q: {}, r: {}, nr: {}, is placed on non-coast sector", q, r, nr);
        }
        createHarbour(sector, effect, resource);
      }
      else
      {
        SPDLOG_ERROR("sector for predefined harbour q: {}, r: {}, nr: {} does not exist", q, r, nr);
        return false;
      }
      it = harbourPool.erase(it);
    }
    else
    {
      it++;
    }
  }
  return true;
}

bool WorldGenerator::createRandomHarbours(HarbourPool& harbourPool)
{
  std::vector<std::reference_wrapper<Sector>> harbourSectors;
  for (auto& [id, sector] : m_sectorMap)
  {
    if (sector.getTile().getTileObject()->type() == ITileObject::TYPE_COAST)
    {
      if (sector.getOppositeTile()->getTileObject()->type() != ITileObject::TYPE_COAST)
      {
        harbourSectors.emplace_back(sector);
      }
    }
  }
  //////
  std::vector<std::reference_wrapper<Sector>> out;
  size_t nelems = harbourPool.size();
  std::sample(
      harbourSectors.begin(),
      harbourSectors.end(),
      std::back_inserter(out),
      nelems,
      std::mt19937{std::random_device{}()}
  );
  for (auto sector : out)
  {
    std::uniform_int_distribution<int> uniform_dist(0, static_cast<int>(harbourPool.size()) - 1);
    auto randomIndex = uniform_dist(m_randomEngine);
    auto entry = harbourPool.at(static_cast<size_t>(randomIndex));
    auto effect = std::get<4>(entry);
    auto resource = std::get<5>(entry);
    harbourPool.erase(std::next(harbourPool.begin(), randomIndex));

    // place harbour here
    auto harbour = new Harbour{ effect, resource };
    sector.get().setSectorObject(harbour);
  }

  return true;
}
void WorldGenerator::createHarbour(Sector& sector, Harbour::EEffect effect, Harbour::EResource resource)
{
  auto harbour = new Harbour(effect, resource);
  m_harbours.emplace_back(harbour);
  sector.setSectorObject(harbour);
}

#include <spdlog/spdlog.h>
#include "settlers/Tile.h"

#include "settlers/Corner.h"

Tile::Tile(int q, int r) : m_q{ q }, m_r{ r }
{
}

Tile::Tile(const Tile& tile) = default;

Tile::~Tile()
{
}

int Tile::q() const
{
  return m_q;
}

int Tile::r() const
{
  return m_r;
}

int Tile::s() const
{
  return -m_q - m_r;
}

int Tile::id() const
{
  // collisions will start to appear when r >= MAX
  if(m_r >= Tile::MAX)
  {
    SPDLOG_WARN("Tile ID collisions may occur");
  }
  return m_q * Tile::MAX + m_r;
}

void Tile::setType(Tile::EType type)
{
  m_type = type;
}

void Tile::setTriggerValue(int value)
{
  m_triggerValue = value;
}

Tile::EType Tile::getType() const
{
  return m_type;
}

int Tile::getTriggerValue() const
{
  return m_triggerValue;
}
Tile::EType Tile::typeFromString(const std::string& typeStr)
{
  if (typeStr == "desert")
  {
    return TYPE_DESERT;
  }
  if (typeStr == "coast")
  {
    return TYPE_COAST;
  }
  if (typeStr == "mountain")
  {
    return TYPE_MOUNTAIN;
  }
  if (typeStr == "meadow")
  {
    return TYPE_MEADOW;
  }
  if (typeStr == "forest")
  {
    return TYPE_FOREST;
  }
  return TYPE_UNDEFINED;
}
std::vector<Tile> Tile::getAllPossibleNeighbors() const
{
  // a tile has 6 neighbors
  return {
        { m_q, m_r - 1},
        { m_q + 1, m_r - 1 },
        { m_q + 1, m_r },
        { m_q, m_r + 1},
        { m_q - 1, m_r + 1 },
        { m_q - 1, m_r },
      };
}
std::vector<Corner> Tile::getAllPossibleCorners() const
{
  auto q = static_cast<double>(m_q);
  auto r = static_cast<double>(m_r);
  // a tile has 6 corners
  return {
    { q + 1.0 / 3.0, r - 2.0 / 3.0 },
    { q + 2.0 / 3.0, r - 1.0 / 3.0 },
    { q + 1.0 / 3.0, r + 1.0 / 3.0 },
    { q - 1.0 / 3.0, r + 2.0 / 3.0 },
    { q - 2.0 / 3.0, r + 1.0 / 3.0 },
    { q - 1.0 / 3.0, r - 1.0 / 3.0 } };
}
const std::vector<std::reference_wrapper<Tile>> Tile::getNeighbors() const
{
  return m_neighbors;
}
const std::vector<std::reference_wrapper<Corner>> Tile::getCorners() const
{
  return m_corners;
}
void Tile::addNeighbor(Tile& tile)
{
  m_neighbors.emplace_back(tile);
}
void Tile::addCorner(Corner& corner)
{
  m_corners.emplace_back(corner);
}
bool Tile::allNeighborsExist() const
{
  // a hex tile can be surrounded by 6 neighbors at max
  return m_neighbors.size() == 6;
}

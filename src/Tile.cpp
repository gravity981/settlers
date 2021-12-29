#include "settlers/Tile.h"

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
  if(typeStr == "desert")
  {
    return TYPE_DESERT;
  }
  if(typeStr == "coast")
  {
    return TYPE_COAST;
  }
  if(typeStr == "mountain")
  {
    return TYPE_MOUNTAIN;
  }
  if(typeStr == "meadow")
  {
    return TYPE_MEADOW;
  }
  if(typeStr == "forest")
  {
    return TYPE_FOREST;
  }
  return TYPE_UNDEFINED;
}
std::vector<Tile> Tile::getAllPossibleNeighbors() const
{
  // a tile has 6 neighbors
  std::vector<Tile> neighbors{
    {m_q+1,m_r},
    {m_q,m_r+1},
    {m_q+1,m_r+1},
    {m_q-1,m_r},
    {m_q,m_r-1},
    {m_q-1,m_r-1},
  };
  return neighbors;
}

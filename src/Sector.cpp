#include "settlers/Sector.h"

#include <cmath>
#include <spdlog/spdlog.h>

#include "settlers/Edge.h"
#include "settlers/Tile.h"

Sector::Sector(double q1, double r1, double q2, double r2, double q3, double r3, Tile& tile, Edge& edge)
    : m_q1(q1),
      m_r1(r1),
      m_q2(q2),
      m_r2(r2),
      m_q3(q3),
      m_r3(r3),
      m_tile(tile),
      m_edge(edge)
{
}

Sector::~Sector()
{
}

Edge& Sector::getEdge()
{
  return m_edge;
}

Tile& Sector::getTile()
{
  return m_tile;
}

int Sector::id()
{
  double q = (m_q1 + m_q2 + m_q3) / 3;
  double r = (m_r1 + m_r2 + m_r3) / 3;
  return static_cast<int>(round(q * 100) * 1000 * 100 + round(r * 100));
}
ISectorObject* Sector::getSectorObject()
{
  return m_sectorObject;
}
void Sector::setSectorObject(ISectorObject* sectorObject)
{
  if (sectorObject == nullptr)
  {
    return;
  }
  m_sectorObject = sectorObject;
}
Tile* Sector::getOppositeTile()
{
  for(auto& tile : m_edge.getTiles())
  {
    if(tile.get().id() != m_tile.id())
    {
      return &tile.get();
    }
  }
  SPDLOG_ERROR("there is no oppsite tile");
  return nullptr;
}

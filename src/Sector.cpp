#include "settlers/Sector.h"

#include <spdlog/spdlog.h>

#include <cmath>

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
  return id(m_q1, m_r1, m_q2, m_r2, m_q3, m_r3);
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
  for (auto& tile : m_edge.getTiles())
  {
    if (tile.get().id() != m_tile.id())
    {
      return &tile.get();
    }
  }
  SPDLOG_ERROR("there is no oppsite tile");
  return nullptr;
}
int Sector::id(double q, double r, int sectorNr)
{
  if (sectorNr < 0 || sectorNr > 5)
  {
    SPDLOG_ERROR("invalid sectorNr \"{}\", return 0", sectorNr);
    return 0;
  }
  std::vector<std::tuple<double, double, double, double>> sectorCorners{
    std::make_tuple(1.0 / 3.0, -2.0 / 3.0, 2.0 / 3.0, -1.0 / 3.0),
    std::make_tuple(2.0 / 3.0, -1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0),
    std::make_tuple(1.0 / 3.0, 1.0 / 3.0, -1.0 / 3.0, 2.0 / 3.0),
    std::make_tuple(-1.0 / 3.0, 2.0 / 3.0, -2.0 / 3.0, 1.0 / 3.0),
    std::make_tuple(-2.0 / 3.0, 1.0 / 3.0, -1.0 / 3.0, -1.0 / 3.0),
    std::make_tuple(-1.0 / 3.0, -1.0 / 3.0, 1.0 / 3.0, -2.0 / 3.0)
  };
  auto corners = sectorCorners[sectorNr];
  return id(q, r, std::get<0>(corners), std::get<1>(corners), std::get<2>(corners), std::get<3>(corners));
}
int Sector::id(double q1, double r1, double q2, double r2, double q3, double r3)
{
  double q = (q1 + q2 + q3) / 3;
  double r = (r1 + r2 + r3) / 3;
  return static_cast<int>(round(q * 100) * 1000 * 100 + round(r * 100));
}

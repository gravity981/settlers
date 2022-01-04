#include "settlers/Corner.h"

#include <spdlog/spdlog.h>
#include <cmath>

Corner::Corner(double q, double r) : m_q(q), m_r(r)
{
}
Corner::Corner(const Corner& corner) = default;

Corner::~Corner() = default;

std::vector<Corner> Corner::getOverlappingCorners(const std::vector<Corner>& cornersA, const std::vector<Corner>& cornersB)
{
  std::vector<Corner> intersectingCorners;
  for (const auto& cornerA : cornersA)
  {
    for (const auto& cornerB : cornersB)
    {
      if (cornerA == cornerB)
      {
        intersectingCorners.emplace_back(cornerA);
      }
    }
  }
  return intersectingCorners;
}
bool Corner::operator==(const Corner& corner) const
{
  // coordinates must be equal for a corner to be the same
  // return m_q == corner.m_q && m_r == corner.m_r; this does not work for double
  return id() == corner.id();
}
int Corner::id() const
{
  if (m_r >= Corner::MAX)
  {
    SPDLOG_WARN("Tile ID collisions may occur");
  }
  int q = static_cast<int>(round(m_q * 100));
  int r = static_cast<int>(round(m_r * 100));

  return q * Corner::MAX*100 + r;
}
double Corner::q() const
{
  return m_q;
}
double Corner::r() const
{
  return m_r;
}
void Corner::addTile(Tile& tile)
{
  m_tiles.emplace_back(tile);
}
void Corner::addEdge(Edge& edge)
{
  m_edges.emplace_back(edge);
}
void Corner::addSector(Sector& sector)
{
  m_sectors.emplace_back(sector);
}
std::vector<std::reference_wrapper<Edge>>& Corner::getEdges()
{
  return m_edges;
}
std::vector<std::reference_wrapper<Tile>>& Corner::getTiles()
{
  return m_tiles;
}
std::vector<std::reference_wrapper<Sector>>& Corner::getSectors()
{
  return m_sectors;
}
void Corner::setCornerObject(ICornerObject* cornerObject)
{
  if(cornerObject == nullptr)
  {
    return;
  }
  m_cornerObject = cornerObject;
}
ICornerObject* Corner::getCornerObject()
{
  return m_cornerObject;
}

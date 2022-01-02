#include "settlers/Edge.h"

#include <spdlog/spdlog.h>

#include <cmath>

#include "settlers/Corner.h"

Edge::Edge(double q1, double r1, double q2, double r2) : m_q1(q1), m_r1(r1), m_q2(q2), m_r2(r2)
{
}

Edge::~Edge()
{
}
void Edge::addCorner(Corner& corner)
{
  m_corners.emplace_back(corner);
}
std::vector<std::reference_wrapper<Corner>>& Edge::getCorners()
{
  return m_corners;
}

int Edge::id() const
{
  std::vector<Corner> corners{{m_q1,m_r1}, {m_q2, m_r2}};
  return id(corners);
}

int Edge::id(std::vector<Corner>& corners)
{
  if (corners.empty())
  {
    SPDLOG_ERROR("no corners, cannot calculate edge id");
    return 0;
  }
  double q{ 0 }, r{ 0 };
  for (const auto& corner : corners)
  {
    q += corner.q();
    r += corner.r();
  }
  q /= static_cast<double>(corners.size());
  r /= static_cast<double>(corners.size());
  int id = static_cast<int>(round(q * 100) * 1000 * 100 + round(r * 100));
  return id;
}
void Edge::addTile(Tile& tile)
{
  m_tiles.emplace_back(tile);
}

std::vector<std::reference_wrapper<Tile>>& Edge::getTiles()
{
  return m_tiles;
}

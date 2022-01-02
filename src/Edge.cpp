#include "settlers/Edge.h"
#include "settlers/Corner.h"

#include <spdlog/spdlog.h>
#include <cmath>


Edge::Edge()
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
  std::vector<Corner> tmp;
  for (auto& corner : m_corners)
  {
    tmp.emplace_back(Corner{ corner.get().q(), corner.get().r() });
  }
  return id(tmp);
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

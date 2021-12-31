#include "settlers/Edge.h"

#include <spdlog/spdlog.h>

#include <cmath>

#include "settlers/Corner.h"

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
std::vector<std::reference_wrapper<Corner>> Edge::getCorners()
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
    SPDLOG_ERROR("no corners, cannot calulate edge id");
    return 0;
  }
  double q{ 0 }, r{ 0 };
  for (const auto& corner : corners)
  {
    q += corner.q();
    r += corner.r();
  }
  q /= corners.size();
  r /= corners.size();
  int id = static_cast<int>(round(q * 100) * 1000 * 100 + round(r * 100));
  return id;
}

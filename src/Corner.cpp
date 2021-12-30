#include "settlers/Corner.h"

Corner::Corner(double q, double r):m_q(q), m_r(r)
{
}
Corner::Corner(const Corner& corner) = default;

Corner::~Corner() = default;

std::vector<Corner> Corner::getIntersectingCorners(const std::vector<Corner>& cornersA, const std::vector<Corner>& cornersB)
{
  std::vector<Corner> intersectingCorners;
  for(const auto& cornerA : cornersA) {
    for(const auto& cornerB : cornersB) {
      if(cornerA == cornerB) {
        intersectingCorners.push_back(cornerA);
      }
    }
  }
  return intersectingCorners;
}
bool Corner::operator==(const Corner& corner) const
{
  //coordinates must be equal for a corner to be the same
  return m_q == corner.m_q && m_r == corner.m_r;
}


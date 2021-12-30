#pragma once

#include "Edge.h"
#include "Tile.h"

#include <vector>
#include <functional>

class Corner
{
 private:
  const double m_q;
  const double m_r;
  std::vector<std::reference_wrapper<Edge>> m_edges;
  std::vector<std::reference_wrapper<Tile>> m_tiles;
 public:
  Corner(double q, double r);
  Corner(const Corner& corner);
  virtual ~Corner();

  bool operator==(const Corner& corner) const;

  static std::vector<Corner> getIntersectingCorners(const std::vector<Corner>& cornersA,const std::vector<Corner>& cornersB);
};

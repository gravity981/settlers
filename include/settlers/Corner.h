#pragma once

#include "Edge.h"
#include "Tile.h"

#include <vector>
#include <functional>

class Corner
{
 private:
  std::vector<std::reference_wrapper<Edge>> m_edges;
  std::vector<std::reference_wrapper<Tile>> m_tiles;
 public:
  explicit Corner();
  virtual ~Corner();
};

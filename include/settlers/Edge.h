#pragma once

#include "Tile.h"

#include <functional>
#include <vector>

class Corner;

class Edge
{
 private:
  std::vector<std::reference_wrapper<Corner>> m_corners;
  std::vector<std::reference_wrapper<Tile>> m_tiles;

 public:
  Edge();
  virtual ~Edge();
  int id() const;
  void addCorner(Corner& corner);
  std::vector<std::reference_wrapper<Corner>>& getCorners();
  void addTile(Tile& tile);
  std::vector<std::reference_wrapper<Tile>>& getTiles();

  static int id(std::vector<Corner>& corners);
};

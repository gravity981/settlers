#pragma once

#include <functional>
#include <vector>

#include "IEdgeObject.h"
#include "Tile.h"

class Corner;

class Edge
{
 private:
  double m_q1, m_r1, m_q2, m_r2;
  std::vector<std::reference_wrapper<Corner>> m_corners;
  std::vector<std::reference_wrapper<Tile>> m_tiles;

  IEdgeObject* m_edgeObject;

 public:
  Edge(double q1, double r1, double q2, double r2);
  virtual ~Edge();
  int id() const;
  void addCorner(Corner& corner);
  std::vector<std::reference_wrapper<Corner>>& getCorners();
  void addTile(Tile& tile);
  std::vector<std::reference_wrapper<Tile>>& getTiles();
  void setEdgeObject(IEdgeObject* edgeObject);
  IEdgeObject* getEdgeObject();

  static int id(std::vector<Corner>& corners);
};

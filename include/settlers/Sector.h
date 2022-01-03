#pragma once

#include <vector>

#include "ISectorObject.h"

class Edge;
class Tile;

class Sector
{
 private:
  double m_q1, m_r1;
  double m_q2, m_r2;
  double m_q3, m_r3;
  Tile& m_tile;
  Edge& m_edge;
  ISectorObject* m_sectorObject{ nullptr };

 public:
  Sector(double q1, double r1, double q2, double r2, double q3, double r3, Tile& tile, Edge& edge);
  virtual ~Sector();

  int id();

  Edge& getEdge();
  Tile& getTile();
  Tile* getOppositeTile();

  void setSectorObject(ISectorObject* sectorObject);
  ISectorObject* getSectorObject();
};

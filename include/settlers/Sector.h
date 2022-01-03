#pragma once

#include <vector>

#include "ISectorObject.h"

class Corner;
class Tile;

class Sector
{
 private:
  double m_q1, m_r1;
  double m_q2, m_r2;
  double m_q3, m_r3;
  std::vector<std::reference_wrapper<Corner>> m_corners;
  Tile& m_tile;
  ISectorObject* m_sectorObject{ nullptr };

 public:
  Sector(double q1, double r1, double q2, double r2, double q3, double r3, Tile& tile);
  virtual ~Sector();

  int id();

  void addCorner(Corner& corner);
  std::vector<std::reference_wrapper<Corner>>& getCorners();
  Tile& getTile();

  void setSectorObject(ISectorObject* sectorObject);
  ISectorObject* getSectorObject();
};

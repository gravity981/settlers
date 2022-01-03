#pragma once

#include <functional>
#include <vector>

#include "Edge.h"
#include "ICornerObject.h"
#include "Sector.h"
#include "Tile.h"

class Corner
{
 public:
  static const int MAX = 1000;

 private:
  const double m_q;
  const double m_r;
  std::vector<std::reference_wrapper<Edge>> m_edges;
  std::vector<std::reference_wrapper<Tile>> m_tiles;
  std::vector<std::reference_wrapper<Sector>> m_sectors;

  ICornerObject* m_cornerObject{ nullptr };

 public:
  Corner(double q, double r);
  Corner(const Corner& corner);
  virtual ~Corner();

  bool operator==(const Corner& corner) const;
  double q() const;
  double r() const;
  int id() const;
  void addEdge(Edge& edge);
  void addTile(Tile& tile);
  void addSector(Sector& sector);

  static std::vector<Corner> getOverlappingCorners(const std::vector<Corner>& cornersA, const std::vector<Corner>& cornersB);
};

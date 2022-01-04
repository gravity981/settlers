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
  std::vector<std::reference_wrapper<Edge>>& getEdges();
  void addEdge(Edge& edge);
  std::vector<std::reference_wrapper<Tile>>& getTiles();
  void addTile(Tile& tile);
  std::vector<std::reference_wrapper<Sector>>& getSectors();
  void addSector(Sector& sector);
  void setCornerObject(ICornerObject* cornerObject);
  ICornerObject* getCornerObject();

  static std::vector<Corner> getOverlappingCorners(const std::vector<Corner>& cornersA, const std::vector<Corner>& cornersB);
};

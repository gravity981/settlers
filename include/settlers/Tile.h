#pragma once

#include <functional>
#include <string>
#include <vector>

#include "ITileObject.h"
#include "Sector.h"

class Corner;
class Edge;

class Tile
{
 public:
  static const int MAX = 100;

 private:
  const int m_q;
  const int m_r;
  std::vector<std::reference_wrapper<Corner>> m_corners;
  std::vector<std::reference_wrapper<Edge>> m_edges;
  std::vector<std::reference_wrapper<Sector>> m_sectors;

  ITileObject* m_tileObject{ nullptr };

 public:
  Tile(int q, int r);
  Tile(const Tile& tile);
  virtual ~Tile();

  int q() const;
  int r() const;
  int s() const;
  int id() const;

  std::vector<Tile> getAllPossibleNeighbors() const;
  std::vector<Corner> getAllPossibleCorners() const;
  std::vector<Edge> getAllPossibleEdges() const;

  void addCorner(Corner& corner);
  std::vector<std::reference_wrapper<Corner>>& getCorners();
  void addEdge(Edge& edge);
  std::vector<std::reference_wrapper<Edge>>& getEdges();
  void addSector(Sector& sector);
  std::vector<std::reference_wrapper<Sector>>& getSectors();

  void setTileObject(ITileObject* tileObject);
  ITileObject* getTileObject();
};

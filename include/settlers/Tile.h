#pragma once

#include "ITileObject.h"

#include <string>
#include <vector>
#include <functional>

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
  std::vector<std::reference_wrapper<Tile>> m_neighbors;
  std::vector<std::reference_wrapper<Edge>> m_edges;

  ITileObject* m_tileObject{nullptr};

 public:
  Tile(int q, int r);
  Tile(const Tile &tile);
  virtual ~Tile();

  int q() const;
  int r() const;
  int s() const;
  int id() const;

  std::vector<Tile> getAllPossibleNeighbors() const;
  std::vector<Corner> getAllPossibleCorners() const;
  std::vector<Edge> getAllPossibleEdges() const;

  void addNeighbor(Tile& tile);
  std::vector<std::reference_wrapper<Tile>>& getNeighbors();

  void addCorner(Corner& corner);
  std::vector<std::reference_wrapper<Corner>>& getCorners();

  void addEdge(Edge& edge);
  std::vector<std::reference_wrapper<Edge>>& getEdges();


  bool allNeighborsExist() const;

  void setTileObject(ITileObject* tileObject);
  ITileObject* getTileObject();

};

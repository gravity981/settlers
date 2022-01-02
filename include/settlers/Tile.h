#pragma once

#include "ITileObject.h"

#include <string>
#include <vector>
#include <functional>

class Corner;

class Tile
{
 public:
  static const int MAX = 100;

 private:
  const int m_q;
  const int m_r;
  std::vector<std::reference_wrapper<Corner>> m_corners;
  std::vector<std::reference_wrapper<Tile>> m_neighbors;

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

  const std::vector<std::reference_wrapper<Tile>> getNeighbors() const;
  const std::vector<std::reference_wrapper<Corner>> getCorners() const;

  void addNeighbor(Tile& tile);
  void addCorner(Corner& corner);
  bool allNeighborsExist() const;

  void setTileObject(ITileObject* tileObject);
  ITileObject* getTileObject();

};

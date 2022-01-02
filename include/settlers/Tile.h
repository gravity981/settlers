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
  using EType = enum EType {
    TYPE_UNDEFINED,
    TYPE_DESERT,
    TYPE_COAST,
    TYPE_MOUNTAIN,
    TYPE_MEADOW,
    TYPE_FOREST
  };
 private:
  const int m_q;
  const int m_r;
  std::vector<std::reference_wrapper<Corner>> m_corners;
  std::vector<std::reference_wrapper<Tile>> m_neighbors;

  ITileObject* m_tileObject{nullptr};

  EType m_type{TYPE_UNDEFINED};
  int m_triggerValue{0};

 public:
  Tile(int q, int r);
  Tile(const Tile &tile);
  virtual ~Tile();

  int q() const;
  int r() const;
  int s() const;
  int id() const;

  EType getType() const;
  int getTriggerValue() const;
  std::vector<Tile> getAllPossibleNeighbors() const;
  std::vector<Corner> getAllPossibleCorners() const;

  void setType(EType type);
  void setTriggerValue(int value);

  const std::vector<std::reference_wrapper<Tile>> getNeighbors() const;
  const std::vector<std::reference_wrapper<Corner>> getCorners() const;

  void addNeighbor(Tile& tile);
  void addCorner(Corner& corner);
  bool allNeighborsExist() const;

  static EType typeFromString(const std::string& typeStr);
};

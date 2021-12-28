#pragma once

#include <string>
#include <vector>

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
  std::vector<Tile> getNeighbors() const;

  void setType(EType type);
  void setTriggerValue(int value);

  static EType typeFromString(const std::string& typeStr);
};

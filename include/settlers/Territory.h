#pragma once

#include "ITileObject.h"
#include "Tile.h"

#include <string>

class Territory: public ITileObject
{
 public:
  using EType = enum EType {
    TYPE_UNDEFINED,
    TYPE_DESERT,
    TYPE_COAST,
    TYPE_MOUNTAIN,
    TYPE_MEADOW,
    TYPE_FOREST,
    TYPE_FIELD,
    TYPE_HILLS
  };

  Tile& m_tile;

  int m_triggerValue{0};
  EType m_type{TYPE_UNDEFINED};

 public:
  Territory(Tile& tile);
  ~Territory() override;

  int getTriggerValue() const;
  void setTriggerValue(int value);
  EType getType() const;
  void setType(EType type);

  static EType typeFromString(const std::string& typeStr);
};

#pragma once

#include "ITileObject.h"
#include "Tile.h"

#include <string>

class Territory: public ITileObject
{
 public:
  Tile& m_tile;

  int m_triggerValue{0};
  EType m_type;

 public:
  Territory(Tile& tile, ITileObject::EType type);
  ~Territory() override;

  int getTriggerValue() const;
  void setTriggerValue(int value);
  ITileObject::EType type() const override;

  static EType typeFromString(const std::string& typeStr);
};

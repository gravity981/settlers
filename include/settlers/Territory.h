#pragma once

#include "ITileObject.h"
#include "Tile.h"

#include <string>

class Territory: public ITileObject
{
 public:
  Tile& m_tile;

  int m_triggerValue{0};
  EType m_type{TYPE_UNDEFINED};

 public:
  Territory(Tile& tile);
  ~Territory() override;

  int getTriggerValue() const;
  void setTriggerValue(int value);
  ITileObject::EType type() const override;
  void setType(EType type);

  static EType typeFromString(const std::string& typeStr);
};

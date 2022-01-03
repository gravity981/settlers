#pragma once

class ITileObject
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
  virtual ~ITileObject() {}

  virtual ITileObject::EType type() const = 0;
};
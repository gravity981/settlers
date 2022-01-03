#include "settlers/Territory.h"

#include <spdlog/spdlog.h>

Territory::Territory(Tile& tile) : m_tile(tile)
{
}

Territory::~Territory()
{
}

void Territory::setType(Territory::EType type)
{
  m_type = type;
}

void Territory::setTriggerValue(int value)
{
  m_triggerValue = value;
}

ITileObject::EType Territory::type() const
{
  return m_type;
}

int Territory::getTriggerValue() const
{
  return m_triggerValue;
}

Territory::EType Territory::typeFromString(const std::string& typeStr)
{
  if (typeStr == "desert")
  {
    return TYPE_DESERT;
  }
  if (typeStr == "coast")
  {
    return TYPE_COAST;
  }
  if (typeStr == "mountain")
  {
    return TYPE_MOUNTAIN;
  }
  if (typeStr == "meadow")
  {
    return TYPE_MEADOW;
  }
  if (typeStr == "forest")
  {
    return TYPE_FOREST;
  }
  if (typeStr == "hills")
  {
    return TYPE_HILLS;
  }
  if (typeStr == "field")
  {
    return TYPE_FIELD;
  }
  SPDLOG_WARN("type \"{}\" is undefined", typeStr);
  return TYPE_UNDEFINED;
}

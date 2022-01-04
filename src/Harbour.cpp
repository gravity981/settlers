#include "settlers/Harbour.h"

#include <spdlog/spdlog.h>

Harbour::Harbour(EEffect effect, Resource::EResource resource) : m_effect(effect), m_resource(resource)
{
}

Harbour::~Harbour()
{
}

Resource::EResource Harbour::ResourceFromString(const std::string& resourceStr)
{
  if (resourceStr == "clay")
  {
    return Resource::RESOURCE_CLAY;
  }
  if (resourceStr == "sheep")
  {
    return Resource::RESOURCE_SHEEP;
  }
  if (resourceStr == "stone")
  {
    return Resource::RESOURCE_STONE;
  }
  if (resourceStr == "wheat")
  {
    return Resource::RESOURCE_WHEAT;
  }
  if (resourceStr == "wood")
  {
    return Resource::RESOURCE_WOOD;
  }
  if (resourceStr == "all")
  {
    return Resource::RESOURCE_ALL;
  }
  SPDLOG_WARN("resource \"{}\" is undefined", resourceStr);
  return Resource::RESOURCE_UNDEFINED;
}

Harbour::EEffect Harbour::EffectFromString(const std::string& effectStr)
{
  if(effectStr == "3:1")
  {
    return EFFECT_3_FOR_1;
  }
  if(effectStr == "2:1")
  {
    return EFFECT_2_FOR_1;
  }
  SPDLOG_WARN("effect \"{}\" is undefined", effectStr);
  return EFFECT_UNDEFINED;
}

#include "settlers/Harbour.h"

#include <spdlog/spdlog.h>

Harbour::Harbour(EEffect effect, Resource::EResource resource) : m_effect(effect), m_resource(resource)
{
}

Harbour::~Harbour()
{
}

Harbour::EEffect Harbour::EffectFromString(const std::string& effectStr)
{
  if (effectStr == "3:1")
  {
    return EFFECT_3_FOR_1;
  }
  if (effectStr == "2:1")
  {
    return EFFECT_2_FOR_1;
  }
  SPDLOG_WARN("effect \"{}\" is undefined", effectStr);
  return EFFECT_UNDEFINED;
}

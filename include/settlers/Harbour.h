#pragma once

#include <string>

#include "ISectorObject.h"

class Harbour : public ISectorObject
{
 public:
  using EEffect = enum EEffect {
    EFFECT_3_FOR_1,
    EFFECT_2_FOR_1,
    EFFECT_UNDEFINED
  };
  using EResource = enum EResource {
    RESOURCE_CLAY,
    RESOURCE_STONE,
    RESOURCE_WHEAT,
    RESOURCE_SHEEP,
    RESOURCE_WOOD,
    RESOURCE_ALL,
    RESOURCE_UNDEFINED
  };

 private:
  EEffect m_effect;
  EResource m_resource;

 public:
  explicit Harbour(EEffect effect, EResource resource);
  virtual ~Harbour();

  static EResource ResourceFromString(const std::string& resourceStr);
  static EEffect EffectFromString(const std::string& effectStr);
};

#pragma once

#include <string>

#include "ISectorObject.h"
#include "Resource.h"

class Harbour : public ISectorObject
{
 public:
  using EEffect = enum EEffect {
    EFFECT_3_FOR_1,
    EFFECT_2_FOR_1,
    EFFECT_UNDEFINED
  };

 private:
  EEffect m_effect;
  Resource::EResource m_resource;

 public:
  explicit Harbour(EEffect effect, Resource::EResource resource);
  virtual ~Harbour();

  static Resource::EResource ResourceFromString(const std::string& resourceStr);
  static EEffect EffectFromString(const std::string& effectStr);
};

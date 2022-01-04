#pragma once

#include <map>

class TriggerEffectCollection
{
 public:
  using ETriggerEffect = enum ETriggerEffect{
    TRIGGEREFFECT_GIVE_RESOURCE,
    TRIGGEREFFECT_INIT_RAID,
    TRIGGEREFFECT_UNDEFINED
  };
 private:
  std::map<int, ETriggerEffect> m_triggerEffects;
 public:
  explicit TriggerEffectCollection();
  virtual ~TriggerEffectCollection();

  void addEffect(int triggerValue, ETriggerEffect);
  ETriggerEffect getEffect(int triggerValue) const;
  int count() const;

  static ETriggerEffect strToEffect(const std::string& effectStr);
};

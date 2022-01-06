#include "settlers/TriggerEffectCollection.h"

#include <spdlog/spdlog.h>

TriggerEffectCollection::TriggerEffectCollection()
{
}

TriggerEffectCollection::~TriggerEffectCollection()
{
}
void TriggerEffectCollection::addEffect(int triggerValue, TriggerEffectCollection::ETriggerEffect effect)
{
  if (m_triggerEffects.find(triggerValue) != m_triggerEffects.end())
  {
    SPDLOG_WARN("cannot add effect. there is already a registered effect for triggerValue \"{}\"", triggerValue);
    return;
  }
  m_triggerEffects.insert(std::make_pair(triggerValue, effect));
}
TriggerEffectCollection::ETriggerEffect TriggerEffectCollection::getEffect(int triggerValue) const
{
  if (m_triggerEffects.find(triggerValue) == m_triggerEffects.end())
  {
    return TRIGGEREFFECT_UNDEFINED;
  }
  return m_triggerEffects.at(triggerValue);
}
TriggerEffectCollection::ETriggerEffect TriggerEffectCollection::strToEffect(const std::string& effectStr)
{
  if (effectStr == "resource")
  {
    return TRIGGEREFFECT_GIVE_RESOURCE;
  }
  if (effectStr == "raid")
  {
    return TRIGGEREFFECT_INIT_RAID;
  }
  SPDLOG_WARN("effect \"{}\" is undefined", effectStr);
  return TRIGGEREFFECT_UNDEFINED;
}
int TriggerEffectCollection::count() const
{
  return static_cast<int>(m_triggerEffects.size());
}

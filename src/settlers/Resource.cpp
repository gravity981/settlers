#include "settlers/Resource.h"

#include <spdlog/spdlog.h>

Resource::Resource()
{
}

Resource::~Resource()
{
}
int Resource::get(Resource::EResource resource) const
{
  if (m_resourceMap.find(resource) == m_resourceMap.end())
  {
    return 0;
  }
  else
  {
    return m_resourceMap.at(resource);
  }
}
void Resource::set(Resource::EResource resource, int value)
{
  if (m_resourceMap.find(resource) == m_resourceMap.end())
  {
    m_resourceMap.insert(std::make_pair(resource, value));
  }
  else
  {
    m_resourceMap.at(resource) = value;
  }
}
Resource& Resource::operator+=(const Resource& rhs)
{
  auto& lhs = *this;
  for (auto& [resource, value] : rhs.m_resourceMap)
  {
    lhs.add(resource, value);
  }
  return lhs;
}
Resource& Resource::operator-=(const Resource& rhs)
{
  auto& lhs = *this;
  for (auto& [resource, value] : rhs.m_resourceMap)
  {
    lhs.subtract(resource, value);
  }
  return lhs;
}
Resource operator+(Resource lhs, const Resource& rhs)
{
  lhs += rhs;
  return lhs;
}
Resource operator-(Resource lhs, const Resource& rhs)
{
  lhs -= rhs;
  return lhs;
}

void Resource::add(Resource::EResource resource, int value)
{
  if (m_resourceMap.find(resource) == m_resourceMap.end())
  {
    m_resourceMap.insert(std::make_pair(resource, value));
  }
  else
  {
    m_resourceMap.at(resource) += value;
  }
}
void Resource::subtract(Resource::EResource resource, int value)
{
  if (m_resourceMap.find(resource) == m_resourceMap.end())
  {
    m_resourceMap.insert(std::make_pair(resource, -value));
  }
  else
  {
    m_resourceMap.at(resource) -= value;
  }
}
bool operator<(const Resource& lhs, const Resource& rhs)
{
  bool smaller = true;
  for (const auto& [resource, lhsVal] : lhs.m_resourceMap)
  {
    auto rhsVal = rhs.get(resource);
    if (lhsVal >= rhsVal)
    {
      smaller = false;
      break;
    }
  }
  return smaller;
}
bool operator>(const Resource& lhs, const Resource& rhs)
{
  return rhs < lhs;
}
bool operator<=(const Resource& lhs, const Resource& rhs)
{
  return !(lhs > rhs);
}
bool operator>=(const Resource& lhs, const Resource& rhs)
{
  return !(lhs < rhs);
}
Resource::EResource Resource::strToResource(const std::string& resourceStr)
{
  if (resourceStr == "clay")
  {
    return RESOURCE_CLAY;
  }
  if (resourceStr == "sheep")
  {
    return RESOURCE_SHEEP;
  }
  if (resourceStr == "stone")
  {
    return RESOURCE_STONE;
  }
  if (resourceStr == "wheat")
  {
    return RESOURCE_WHEAT;
  }
  if (resourceStr == "wood")
  {
    return RESOURCE_WOOD;
  }
  if (resourceStr == "all")
  {
    return RESOURCE_ALL;
  }
  SPDLOG_WARN("resource \"{}\" is undefined", resourceStr);
  return RESOURCE_UNDEFINED;
}
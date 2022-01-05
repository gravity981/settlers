#pragma once

#include <map>

class Resource
{
 public:
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
  std::map<EResource, int> m_resourceMap;
 public:
  explicit Resource();
  virtual ~Resource();

  int get(EResource resource) const;
  void set(EResource resource, int value);

  Resource& operator+=(const Resource& rhs);
  Resource& operator-=(const Resource& rhs);
  friend Resource operator+(Resource lhs, const Resource& rhs);
  friend Resource operator-(Resource lhs, const Resource& rhs);
  friend bool operator<(const Resource& lhs, const Resource& rhs);
  friend bool operator>(const Resource& lhs, const Resource& rhs);
  friend bool operator<=(const Resource& lhs, const Resource& rhs);
  friend bool operator>=(const Resource& lhs, const Resource& rhs);

 private:
  void add(EResource resource, int value);
  void subtract(EResource resource, int value);

};

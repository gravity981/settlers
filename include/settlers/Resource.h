#pragma once

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
  EResource m_resource;
  int amount;
 public:
  explicit Resource();
  virtual ~Resource();
};

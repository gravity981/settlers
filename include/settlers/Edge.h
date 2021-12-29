#pragma once

#include <vector>
#include <functional>

class Corner;

class Edge
{
 private:
  std::vector<std::reference_wrapper<Corner>> m_corners;
 public:
  explicit Edge();
  virtual ~Edge();
};

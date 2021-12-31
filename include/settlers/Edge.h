#pragma once

#include <functional>
#include <vector>

class Corner;

class Edge
{
 private:
  std::vector<std::reference_wrapper<Corner>> m_corners;

 public:
  Edge();
  virtual ~Edge();
  void addCorner(Corner& corner);
  std::vector<std::reference_wrapper<Corner>> getCorners();
  int id() const;
  static int id(std::vector<Corner>& corners);
};

#pragma once
#include "Tile.h"

class Territory: public Tile
{
 public:
  Territory(int q, int r);
  virtual ~Territory();
};

#pragma once

#include "Corner.h"

class Settlement: public Corner
{
 public:
  Settlement(double q, double r);
  virtual ~Settlement();
};

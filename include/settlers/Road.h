#pragma once

#include "IEdgeObject.h"

class Road: public IEdgeObject
{
 public:
  Road();
  ~Road() override;
};

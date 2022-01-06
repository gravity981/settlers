#pragma once

#include "settlers/IWorld.h"

#include <gmock/gmock.h>

class MockWorld: public IWorld
{
 public:
  MOCK_METHOD2(generateFromFile, bool(const std::string&, unsigned long));

};
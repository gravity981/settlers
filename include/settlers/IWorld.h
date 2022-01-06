#pragma once

class IWorld
{
 public:
  virtual ~IWorld()
  {
  }

  virtual bool generateFromFile(const std::string& filePath, unsigned long seed) = 0;
};

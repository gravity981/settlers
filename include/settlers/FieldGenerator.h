#pragma once

#include <string>

class FieldGenerator
{
 public:
  explicit FieldGenerator();
  virtual ~FieldGenerator();

  bool generateFromFile(const std::string& filePath);
};

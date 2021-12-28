#include "settlers/FieldGenerator.h"

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

FieldGenerator::FieldGenerator()
{
}

FieldGenerator::~FieldGenerator()
{
}
bool FieldGenerator::generateFromFile(const std::string& filePath)
{
  std::ifstream ifs;
  ifs.open(filePath, std::ifstream::in);
  if (ifs.fail())
  {
    return false;
  }
  nlohmann::json jsonData;
  bool success = false;
  try
  {
    ifs >> jsonData;
    success = true;
  }
  catch (nlohmann::json::parse_error& ex)
  {
    std::cerr << "parse error at byte " << ex.byte << std::endl;
  }
  ifs.close();
  if (success)
  {
    std::cout << jsonData.dump(4) << std::endl;
  }
  return success;
}

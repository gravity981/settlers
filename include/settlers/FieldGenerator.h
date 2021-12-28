#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include "Tile.h"

class FieldGenerator
{
 private:
  std::map<int, Tile> m_tileMap;
 public:
  explicit FieldGenerator();
  virtual ~FieldGenerator();

  bool generateFromFile(const std::string& filePath);

 private:
  static bool readFile(const std::string& filePath, nlohmann::json& jsonData);
  bool generateTiles(nlohmann::json jsonData);
};

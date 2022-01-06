
#include <spdlog/spdlog.h>

#include <iostream>

#include "settlers/World.h"

int main(int argc, char *argv[])
{
  spdlog::set_level(spdlog::level::debug); // Set global log level to debug
  spdlog::set_pattern("[%H:%M:%S.%e %z][%l][%s][%!()][line %#] %v");
  if (argc < 2)
  {
    SPDLOG_ERROR("wrong number of parameters. abort");
    return 1;
  }
  World world;
  std::string filePath = argv[1];
  auto seed = std::chrono::system_clock::now().time_since_epoch().count();
  auto success = world.generateFromFile(filePath, seed);
  if(success)
  {
    SPDLOG_INFO("generated game world");
  }
  else
  {
    SPDLOG_ERROR("could not generate game world");
  }

  return 0;
}
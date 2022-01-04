#include "settlers/TradingCenter.h"

#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

TEST(ResourceTests, getCorrectPossibleEdges)
{
  TradingCenter tradingCenter;
  EXPECT_TRUE(tradingCenter.initFromFile("data/building_costs_original.json"));
}

int main(int argc, char **argv)
{
  spdlog::set_level(spdlog::level::debug); // Set global log level to debug
  spdlog::set_pattern("[%H:%M:%S.%e %z][%l][%s][%!()][line %#] %v");
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
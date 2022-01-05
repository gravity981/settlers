#include "settlers/TradingCenter.h"

#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

TEST(TradingCenterTests, initFromOriginalFileShouldWork)
{
  TradingCenter tradingCenter;
  ASSERT_TRUE(tradingCenter.initFromFile("data/building_costs_original.json"));

  auto resource = tradingCenter.getCostOf(TradingCenter::BUYABLEOBJECT_ROAD);
  EXPECT_EQ(resource.get(Resource::RESOURCE_WOOD), 1);
  EXPECT_EQ(resource.get(Resource::RESOURCE_CLAY), 1);
  EXPECT_EQ(resource.get(Resource::RESOURCE_WHEAT), 0);
  EXPECT_EQ(resource.get(Resource::RESOURCE_STONE), 0);
  EXPECT_EQ(resource.get(Resource::RESOURCE_SHEEP), 0);
  EXPECT_EQ(resource.get(Resource::RESOURCE_UNDEFINED), 0);
  EXPECT_EQ(resource.get(Resource::RESOURCE_ALL), 0);

  resource = tradingCenter.getCostOf(TradingCenter::BUYABLEOBJECT_VILLAGE);
  EXPECT_EQ(resource.get(Resource::RESOURCE_WOOD), 1);
  EXPECT_EQ(resource.get(Resource::RESOURCE_CLAY), 1);
  EXPECT_EQ(resource.get(Resource::RESOURCE_WHEAT), 1);
  EXPECT_EQ(resource.get(Resource::RESOURCE_STONE), 0);
  EXPECT_EQ(resource.get(Resource::RESOURCE_SHEEP), 1);
  EXPECT_EQ(resource.get(Resource::RESOURCE_UNDEFINED), 0);
  EXPECT_EQ(resource.get(Resource::RESOURCE_ALL), 0);

  resource = tradingCenter.getCostOf(TradingCenter::BUYABLEOBJECT_CITY);
  EXPECT_EQ(resource.get(Resource::RESOURCE_WOOD), 0);
  EXPECT_EQ(resource.get(Resource::RESOURCE_CLAY), 0);
  EXPECT_EQ(resource.get(Resource::RESOURCE_WHEAT), 2);
  EXPECT_EQ(resource.get(Resource::RESOURCE_STONE), 3);
  EXPECT_EQ(resource.get(Resource::RESOURCE_SHEEP), 0);
  EXPECT_EQ(resource.get(Resource::RESOURCE_UNDEFINED), 0);
  EXPECT_EQ(resource.get(Resource::RESOURCE_ALL), 0);

  resource = tradingCenter.getCostOf(TradingCenter::BUYABLEOBJECT_DEVELOPMENT);
  EXPECT_EQ(resource.get(Resource::RESOURCE_WOOD), 0);
  EXPECT_EQ(resource.get(Resource::RESOURCE_CLAY), 0);
  EXPECT_EQ(resource.get(Resource::RESOURCE_WHEAT), 1);
  EXPECT_EQ(resource.get(Resource::RESOURCE_STONE), 1);
  EXPECT_EQ(resource.get(Resource::RESOURCE_SHEEP), 1);
  EXPECT_EQ(resource.get(Resource::RESOURCE_UNDEFINED), 0);
  EXPECT_EQ(resource.get(Resource::RESOURCE_ALL), 0);
}

int main(int argc, char **argv)
{
  spdlog::set_level(spdlog::level::debug); // Set global log level to debug
  spdlog::set_pattern("[%H:%M:%S.%e %z][%l][%s][%!()][line %#] %v");
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
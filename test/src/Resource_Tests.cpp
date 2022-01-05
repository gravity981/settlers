#include "settlers/Resource.h"

#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

TEST(ResourceTests, checkAddResources)
{
  Resource resourceA, resourceB;
  resourceA.set(Resource::RESOURCE_WOOD, 3);
  resourceA.set(Resource::RESOURCE_STONE, -2);

  resourceB.set(Resource::RESOURCE_WOOD, 5);
  resourceB.set(Resource::RESOURCE_STONE, 2);
  resourceB.set(Resource::RESOURCE_CLAY, 9);

  resourceA += resourceB;

  EXPECT_EQ(resourceA.get(Resource::RESOURCE_WOOD), 8);
  EXPECT_EQ(resourceA.get(Resource::RESOURCE_STONE), 0);
  EXPECT_EQ(resourceA.get(Resource::RESOURCE_CLAY), 9);
  EXPECT_EQ(resourceA.get(Resource::RESOURCE_WHEAT), 0);

  auto resourceC = resourceA + resourceB;
  EXPECT_EQ(resourceC.get(Resource::RESOURCE_WOOD), 13);
  EXPECT_EQ(resourceC.get(Resource::RESOURCE_STONE), 2);
  EXPECT_EQ(resourceC.get(Resource::RESOURCE_CLAY), 18);
  EXPECT_EQ(resourceC.get(Resource::RESOURCE_WHEAT), 0);
}

TEST(ResourceTests, checkSubtractResources)
{
  Resource resourceA, resourceB;
  resourceA.set(Resource::RESOURCE_WOOD, 3);
  resourceA.set(Resource::RESOURCE_STONE, -2);

  resourceB.set(Resource::RESOURCE_WOOD, 5);
  resourceB.set(Resource::RESOURCE_STONE, 2);
  resourceB.set(Resource::RESOURCE_CLAY, 9);

  resourceA -= resourceB;

  EXPECT_EQ(resourceA.get(Resource::RESOURCE_WOOD), -2);
  EXPECT_EQ(resourceA.get(Resource::RESOURCE_STONE), -4);
  EXPECT_EQ(resourceA.get(Resource::RESOURCE_CLAY), -9);
  EXPECT_EQ(resourceA.get(Resource::RESOURCE_WHEAT), 0);

  auto resourceC = resourceA - resourceB;
  EXPECT_EQ(resourceC.get(Resource::RESOURCE_WOOD), -7);
  EXPECT_EQ(resourceC.get(Resource::RESOURCE_STONE), -6);
  EXPECT_EQ(resourceC.get(Resource::RESOURCE_CLAY), -18);
  EXPECT_EQ(resourceC.get(Resource::RESOURCE_WHEAT), 0);
}

TEST(ResourceTests, checkResourceComparisons)
{
  Resource resourceA, resourceB;
  resourceA.set(Resource::RESOURCE_WOOD, 3);
  resourceA.set(Resource::RESOURCE_STONE, -2);

  resourceB.set(Resource::RESOURCE_WOOD, 5);
  resourceB.set(Resource::RESOURCE_STONE, 2);
  resourceB.set(Resource::RESOURCE_CLAY, 9);

  EXPECT_TRUE(resourceA < resourceB);
}

int main(int argc, char **argv)
{
  spdlog::set_level(spdlog::level::debug); // Set global log level to debug
  spdlog::set_pattern("[%H:%M:%S.%e %z][%l][%s][%!()][line %#] %v");
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
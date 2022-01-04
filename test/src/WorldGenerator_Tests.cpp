#include "settlers/WorldGenerator.h"

#include <gtest/gtest.h>
#include <spdlog/spdlog.h>


TEST(WorldGeneratorTests, generateFromInexistentFileShouldFail)
{
  WorldGenerator fg;
  EXPECT_FALSE(fg.generateFromFile("don't exist", 0));
}

TEST(WorldGeneratorTests, generateFromInvalidFileShouldFail)
{
  WorldGenerator fg;
  EXPECT_FALSE(fg.generateFromFile("data/field_invalidJson.json", 0));
}

TEST(WorldGeneratorTests, generateFromEmptyFileShouldFail)
{
  WorldGenerator fg;
  EXPECT_FALSE(fg.generateFromFile("data/field_emptyFile.json", 0));
}

TEST(WorldGeneratorTests, generateFromExistingFileShouldWork)
{
  WorldGenerator fg;
  EXPECT_TRUE(fg.generateFromFile("data/field1.json", 0));
  EXPECT_EQ(fg.getTiles().size(), 7);
  EXPECT_EQ(fg.getCorners().size(), 12);
  EXPECT_EQ(fg.getEdges().size(), 10);
  EXPECT_EQ(fg.getSectors().size(), 20);
  EXPECT_EQ(fg.getTerritories().size(), 7);
  EXPECT_EQ(fg.getSettlements().size(), 6);
  EXPECT_EQ(fg.getRoads().size(), 4);
  EXPECT_EQ(fg.getHarbours().size(), 3);
}

TEST(WorldGeneratorTests, generateFromExistingFileOriginalShouldWork)
{
  WorldGenerator fg;
  EXPECT_TRUE(fg.generateFromFile("data/field_original.json", 0));
  EXPECT_EQ(fg.getTiles().size(), 37);
  EXPECT_EQ(fg.getCorners().size(), 72);
  EXPECT_EQ(fg.getEdges().size(), 90);
  EXPECT_EQ(fg.getSectors().size(), 180);
  EXPECT_EQ(fg.getTerritories().size(), 37);
  EXPECT_EQ(fg.getSettlements().size(), 54);
  EXPECT_EQ(fg.getRoads().size(), 72);
  EXPECT_EQ(fg.getHarbours().size(), 0);
}

int main(int argc, char **argv)
{
  spdlog::set_level(spdlog::level::debug); // Set global log level to debug
  spdlog::set_pattern("[%H:%M:%S.%e %z][%l][%s][%!()][line %#] %v");
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
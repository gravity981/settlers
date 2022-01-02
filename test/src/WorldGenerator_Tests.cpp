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
  EXPECT_EQ(fg.getEdges().size(), 10);
  EXPECT_EQ(fg.getTiles().size(), 7);
  EXPECT_EQ(fg.getCorners().size(), 12);
}

TEST(WorldGeneratorTests, generateFromExistingFileOriginalShouldWork)
{
  WorldGenerator fg;
  EXPECT_TRUE(fg.generateFromFile("data/field_original.json", 0));
  EXPECT_EQ(fg.getEdges().size(), 90);
  EXPECT_EQ(fg.getTiles().size(), 37);
  EXPECT_EQ(fg.getCorners().size(), 72);
}

int main(int argc, char **argv)
{
  spdlog::set_level(spdlog::level::debug); // Set global log level to debug
  spdlog::set_pattern("[%H:%M:%S.%e %z][%l][%s][%!()][line %#] %v");
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
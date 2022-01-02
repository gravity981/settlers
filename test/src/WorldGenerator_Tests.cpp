#include "settlers/WorldGenerator.h"

#include <gtest/gtest.h>
#include <spdlog/spdlog.h>


TEST(WorldGeneratorTests, generateFromInexistentFileShouldFail)
{
  WorldGenerator fg;
  EXPECT_FALSE(fg.generateFromFile("don't exist"));
}

TEST(WorldGeneratorTests, generateFromInvalidFileShouldFail)
{
  WorldGenerator fg;
  EXPECT_FALSE(fg.generateFromFile("data/field_invalidJson.json"));
}

TEST(WorldGeneratorTests, generateFromEmptyFileShouldFail)
{
  WorldGenerator fg;
  EXPECT_FALSE(fg.generateFromFile("data/field_emptyFile.json"));
}

TEST(WorldGeneratorTests, generateFromExistingFileShouldWork)
{
  WorldGenerator fg;
  EXPECT_TRUE(fg.generateFromFile("data/field1.json"));
  EXPECT_EQ(fg.getRoads().size(), 10);
  EXPECT_EQ(fg.getTiles().size(), 7);
  EXPECT_EQ(fg.getSettlements().size(), 12);
}


int main(int argc, char **argv)
{
  spdlog::set_level(spdlog::level::debug); // Set global log level to debug
  spdlog::set_pattern("[%H:%M:%S.%e %z][%l][%s][%!()][line %#] %v");
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
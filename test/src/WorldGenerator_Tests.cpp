#include "settlers/WorldGenerator.h"

#include <gtest/gtest.h>
#include <spdlog/spdlog.h>


TEST(WorldGeneratorTest, generateFromInexistentFileShouldFail)
{
  WorldGenerator fg;
  EXPECT_FALSE(fg.generateFromFile("don't exist"));
}

TEST(WorldGeneratorTest, generateFromInvalidFileShouldFail)
{
  WorldGenerator fg;
  EXPECT_FALSE(fg.generateFromFile("data/field_invalidJson.json"));
}

TEST(WorldGeneratorTest, generateFromEmptyFileShouldFail)
{
  WorldGenerator fg;
  EXPECT_FALSE(fg.generateFromFile("data/field_emptyFile.json"));
}

TEST(WorldGeneratorTest, generateFromExistingFileShouldWork)
{
  WorldGenerator fg;
  EXPECT_TRUE(fg.generateFromFile("data/field1.json"));
}


int main(int argc, char **argv)
{
  spdlog::set_level(spdlog::level::debug); // Set global log level to debug
  spdlog::set_pattern("[%H:%M:%S.%e %z][%l][%s][%!()][line %#] %v");
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#include "settlers/WorldGenerator.h"

#include <gtest/gtest.h>
#include <spdlog/spdlog.h>


TEST(WorldGeneratorTests, generateFromInexistentFileShouldFail)
{
  WorldGenerator worldGenerator;
  EXPECT_FALSE(worldGenerator.generateFromFile("/path/to/inexistent/file.json", 0));
}

TEST(WorldGeneratorTests, generateFromInvalidFileShouldFail)
{
  WorldGenerator worldGenerator;
  EXPECT_FALSE(worldGenerator.generateFromFile("data/field_invalidJson.json", 0));
}

TEST(WorldGeneratorTests, generateFromEmptyFileShouldFail)
{
  WorldGenerator worldGenerator;
  EXPECT_FALSE(worldGenerator.generateFromFile("data/field_emptyFile.json", 0));
}

TEST(WorldGeneratorTests, generateFromExistingFileShouldWork)
{
  WorldGenerator worldGenerator;
  ASSERT_TRUE(worldGenerator.generateFromFile("data/field1.json", 0));
  EXPECT_EQ(worldGenerator.getTiles().size(), 7);
  EXPECT_EQ(worldGenerator.getCorners().size(), 12);
  EXPECT_EQ(worldGenerator.getEdges().size(), 10);
  EXPECT_EQ(worldGenerator.getSectors().size(), 20);
  EXPECT_EQ(worldGenerator.getTerritories().size(), 7);
  EXPECT_EQ(worldGenerator.getSettlements().size(), 6);
  EXPECT_EQ(worldGenerator.getRoads().size(), 4);
  EXPECT_EQ(worldGenerator.getHarbours().size(), 3);
  EXPECT_EQ(worldGenerator.getTriggerEffectCollection().count(), 3);
}

TEST(WorldGeneratorTests, generateFromExistingFileOriginalShouldWork)
{
  WorldGenerator worldGenerator;
  ASSERT_TRUE(worldGenerator.generateFromFile("data/field_original.json", 0));
  EXPECT_EQ(worldGenerator.getTiles().size(), 37);
  EXPECT_EQ(worldGenerator.getCorners().size(), 72);
  EXPECT_EQ(worldGenerator.getEdges().size(), 90);
  EXPECT_EQ(worldGenerator.getSectors().size(), 180);
  EXPECT_EQ(worldGenerator.getTerritories().size(), 37);
  EXPECT_EQ(worldGenerator.getSettlements().size(), 54);
  EXPECT_EQ(worldGenerator.getRoads().size(), 72);
  EXPECT_EQ(worldGenerator.getHarbours().size(), 0);
  EXPECT_EQ(worldGenerator.getTriggerEffectCollection().count(), 11);
  EXPECT_EQ(worldGenerator.getTriggerEffectCollection().getEffect(7), TriggerEffectCollection::TRIGGEREFFECT_INIT_RAID);
}

int main(int argc, char **argv)
{
  spdlog::set_level(spdlog::level::debug); // Set global log level to debug
  spdlog::set_pattern("[%H:%M:%S.%e %z][%l][%s][%!()][line %#] %v");
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#include "settlers/World.h"

#include <gtest/gtest.h>
#include <spdlog/spdlog.h>


TEST(WorldTests, generateFromInexistentFileShouldFail)
{
  World world;
  EXPECT_FALSE(world.generateFromFile("/path/to/inexistent/file.json", 0));
}

TEST(WorldTests, generateFromInvalidFileShouldFail)
{
  World world;
  EXPECT_FALSE(world.generateFromFile("data/field_invalidJson.json", 0));
}

TEST(WorldTests, generateFromEmptyFileShouldFail)
{
  World world;
  EXPECT_FALSE(world.generateFromFile("data/field_emptyFile.json", 0));
}

TEST(WorldTests, generateFromExistingFileShouldWork)
{
  World world;
  ASSERT_TRUE(world.generateFromFile("data/field1.json", 0));
  EXPECT_EQ(world.getTiles().size(), 7);
  EXPECT_EQ(world.getCorners().size(), 12);
  EXPECT_EQ(world.getEdges().size(), 10);
  EXPECT_EQ(world.getSectors().size(), 20);
  EXPECT_EQ(world.getTerritories().size(), 7);
  EXPECT_EQ(world.getSettlements().size(), 6);
  EXPECT_EQ(world.getRoads().size(), 4);
  EXPECT_EQ(world.getHarbours().size(), 3);
  EXPECT_EQ(world.getTriggerEffectCollection().count(), 3);
}

TEST(WorldTests, generateFromExistingFileOriginalShouldWork)
{
  World world;
  ASSERT_TRUE(world.generateFromFile("data/field_original.json", 0));
  EXPECT_EQ(world.getTiles().size(), 37);
  EXPECT_EQ(world.getCorners().size(), 72);
  EXPECT_EQ(world.getEdges().size(), 90);
  EXPECT_EQ(world.getSectors().size(), 180);
  EXPECT_EQ(world.getTerritories().size(), 37);
  EXPECT_EQ(world.getSettlements().size(), 54);
  EXPECT_EQ(world.getRoads().size(), 72);
  EXPECT_EQ(world.getHarbours().size(), 0);
  EXPECT_EQ(world.getTriggerEffectCollection().count(), 11);
  EXPECT_EQ(world.getTriggerEffectCollection().getEffect(7), TriggerEffectCollection::TRIGGEREFFECT_INIT_RAID);
}

int main(int argc, char **argv)
{
  spdlog::set_level(spdlog::level::debug); // Set global log level to debug
  spdlog::set_pattern("[%H:%M:%S.%e %z][%l][%s][%!()][line %#] %v");
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
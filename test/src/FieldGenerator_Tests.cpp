#include "settlers/FieldGenerator.h"

#include <gtest/gtest.h>


TEST(FieldGeneratorTest, generateFromInexistentFileShouldFail)
{
  FieldGenerator fg;
  EXPECT_FALSE(fg.generateFromFile("don't exist"));
}

TEST(FieldGeneratorTest, generateFromInvalidFileShouldFail)
{
  FieldGenerator fg;
  EXPECT_FALSE(fg.generateFromFile("data/field_invalidJson.json"));
}

TEST(FieldGeneratorTest, generateFromExistingFileShouldWork)
{
  FieldGenerator fg;
  EXPECT_TRUE(fg.generateFromFile("data/field1.json"));
}


int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#include "settlers/Corner.h"

#include <gtest/gtest.h>
#include <spdlog/spdlog.h>


TEST(CornerTest, findCorrectIntersectingCorners)
{
  Tile t{0,0};
  for(const auto& n : t.getAllPossibleNeighbors())
  {
    auto intersectingCorners = Corner::getIntersectingCorners(t.getAllPossibleCorners(), n.getAllPossibleCorners());
    EXPECT_EQ(intersectingCorners.size(), 2);
  }
}

int main(int argc, char **argv)
{
  spdlog::set_level(spdlog::level::debug); // Set global log level to debug
  spdlog::set_pattern("[%H:%M:%S.%e %z][%l][%s][%!()][line %#] %v");
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
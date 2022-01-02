#include "settlers/Tile.h"
#include "settlers/Edge.h"
#include "settlers/Corner.h"

#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

TEST(TileTests, getCorrectPossibleEdges)
{
  Tile tile{0,0};
  auto edges = tile.getAllPossibleEdges();
  EXPECT_EQ(edges.size(), 6);
}

TEST(TileTests, getCorrectPossibleCorners)
{
  Tile tile{0,0};
  auto corners = tile.getAllPossibleCorners();
  EXPECT_EQ(corners.size(), 6);
}

int main(int argc, char **argv)
{
  spdlog::set_level(spdlog::level::debug); // Set global log level to debug
  spdlog::set_pattern("[%H:%M:%S.%e %z][%l][%s][%!()][line %#] %v");
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
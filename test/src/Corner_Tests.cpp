#include "settlers/Corner.h"

#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

class CornerParametrizedTests :public ::testing::TestWithParam<std::tuple<Corner, int>> {};

TEST(CornerTests, findCorrectIntersectingCorners)
{
  Tile t{0,0};
  for(const auto& n : t.getAllPossibleNeighbors())
  {
    auto intersectingCorners = Corner::getOverlappingCorners(t.getAllPossibleCorners(), n.getAllPossibleCorners());
    EXPECT_EQ(intersectingCorners.size(), 2);
  }
}

TEST_P(CornerParametrizedTests, idCalculationShouldWork)
{
  auto c = std::get<0>(GetParam());
  auto id = std::get<1>(GetParam());
  EXPECT_EQ(c.id(), id);
}

INSTANTIATE_TEST_CASE_P(
    CornerTests,
    CornerParametrizedTests,
    ::testing::Values(
        std::make_tuple(Corner{0.333, 0.666}, 3300067),
        std::make_tuple(Corner{-0.666, 0.333}, -6699967)
        ));

int main(int argc, char **argv)
{
  spdlog::set_level(spdlog::level::debug); // Set global log level to debug
  spdlog::set_pattern("[%H:%M:%S.%e %z][%l][%s][%!()][line %#] %v");
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
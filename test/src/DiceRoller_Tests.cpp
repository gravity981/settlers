#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

#include "settlers/DiceRoller.h"

MATCHER_P2(
    IsBetween,
    a,
    b,
    std::string(negation ? "isn't" : "is") + " between " + testing::PrintToString(a) + " and " + testing::PrintToString(b))
{
  return a <= arg && arg <= b;
}

class DiceRoller2d6ParametrizedTests : public ::testing::TestWithParam<std::tuple<int, int>>
{
};
TEST_P(DiceRoller2d6ParametrizedTests, checkPossibleCombinations)
{
  DiceRoller roller{ 2, 1, 6, 0 };
  auto value = std::get<0>(GetParam());
  auto numberOfPossibleCombinations = std::get<1>(GetParam());
  EXPECT_EQ(roller.possibleCombinationsForValue(value), numberOfPossibleCombinations);
}
INSTANTIATE_TEST_CASE_P(
    DiceRollerTests,
    DiceRoller2d6ParametrizedTests,
    ::testing::Values(
        std::make_tuple(1, 0),
        std::make_tuple(2, 1),
        std::make_tuple(3, 2),
        std::make_tuple(4, 3),
        std::make_tuple(5, 4),
        std::make_tuple(6, 5),
        std::make_tuple(7, 6),
        std::make_tuple(8, 5),
        std::make_tuple(9, 4),
        std::make_tuple(10, 3),
        std::make_tuple(11, 2),
        std::make_tuple(12, 1)));

class DiceRoller1d6ParametrizedTests : public ::testing::TestWithParam<std::tuple<int, int>>
{
};
TEST_P(DiceRoller1d6ParametrizedTests, checkPossibleCombinations)
{
  DiceRoller roller{ 1, 1, 6, 0 };
  auto value = std::get<0>(GetParam());
  auto numberOfPossibleCombinations = std::get<1>(GetParam());
  EXPECT_EQ(roller.possibleCombinationsForValue(value), numberOfPossibleCombinations);
}
INSTANTIATE_TEST_CASE_P(
    DiceRollerTests,
    DiceRoller1d6ParametrizedTests,
    ::testing::Values(
        std::make_tuple(1, 1),
        std::make_tuple(2, 1),
        std::make_tuple(3, 1),
        std::make_tuple(4, 1),
        std::make_tuple(5, 1),
        std::make_tuple(6, 1)));

TEST(DiceRollerTests, checkSpecialCombinations)
{
  DiceRoller roller(3, 1, 6, 0);
  EXPECT_EQ(roller.possibleCombinationsForValue(10), 27);
  EXPECT_EQ(roller.possibleCombinationsForValue(14), 15);
  EXPECT_EQ(roller.possibleCombinationsForValue(7), 15);
  EXPECT_EQ(roller.possibleCombinationsForValue(4), 3);
}

TEST(DiceRollerTests, checkRandomDistribution)
{
  int numberOfRolls = 100000;
  // statistically, each value of the 10-sided dice should be rolled 100'000 / 10 = 10'000 times
  // give a margin of +- 200 because number of rolls is not infinite
  int rangeMin = 9800;
  int rageMax = 10200;
  DiceRoller roller(1, 1, 10, DiceRoller::randomSeed());
  std::map<int, int> diceDistribution;
  for (int i = 0; i < numberOfRolls; i++)
  {
    auto value = roller.roll();
    if (diceDistribution.find(value) == diceDistribution.end())
    {
      diceDistribution.insert(std::make_pair(value, 1));
    }
    else
    {
      diceDistribution.at(value) += 1;
    }
  }
  ASSERT_EQ(diceDistribution.size(), 10);
  EXPECT_THAT(diceDistribution[1], IsBetween(rangeMin, rageMax));
  EXPECT_THAT(diceDistribution[2], IsBetween(rangeMin, rageMax));
  EXPECT_THAT(diceDistribution[3], IsBetween(rangeMin, rageMax));
  EXPECT_THAT(diceDistribution[4], IsBetween(rangeMin, rageMax));
  EXPECT_THAT(diceDistribution[5], IsBetween(rangeMin, rageMax));
  EXPECT_THAT(diceDistribution[6], IsBetween(rangeMin, rageMax));
  EXPECT_THAT(diceDistribution[7], IsBetween(rangeMin, rageMax));
  EXPECT_THAT(diceDistribution[8], IsBetween(rangeMin, rageMax));
  EXPECT_THAT(diceDistribution[9], IsBetween(rangeMin, rageMax));
  EXPECT_THAT(diceDistribution[10], IsBetween(rangeMin, rageMax));

}

int main(int argc, char **argv)
{
  spdlog::set_level(spdlog::level::debug);  // Set global log level to debug
  spdlog::set_pattern("[%H:%M:%S.%e %z][%l][%s][%!()][line %#] %v");
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
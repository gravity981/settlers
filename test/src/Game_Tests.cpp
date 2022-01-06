#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

#include "mock/MockGameObserver.h"
#include "mock/MockWorld.h"
#include "settlers/Game.h"

TEST(DISABLED_GameTests, startGameShouldChangeState)
{
  MockWorld world;
  EXPECT_CALL(world, generateFromFile).Times(1);
  MockGameObserver observer;
  EXPECT_CALL(observer, gameStateChanged).Times(1);
  Game game{ world };
  EXPECT_EQ(game.getGameState(), Game::GAMESTATE_IDLE);
  game.registerGameObserver(&observer);
  std::vector<Game::SPlayer> players = { { 1, Player::PLAYERCOLOR_RED },
                                         { 2, Player::PLAYERCOLOR_BLUE },
                                         { 3, Player::PLAYERCOLOR_ORANGE } };
  ASSERT_TRUE(game.start(players, "some/path", "some/path", "some/path" , 0, 0));
}

int main(int argc, char **argv)
{
  spdlog::set_level(spdlog::level::debug);  // Set global log level to debug
  spdlog::set_pattern("[%H:%M:%S.%e %z][%l][%s][%!()][line %#] %v");
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
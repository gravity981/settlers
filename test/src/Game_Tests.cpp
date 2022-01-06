#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

#include "mock/MockWorld.h"
#include "settlers/Game.h"
#include "mock/MockGameObserver.h"

TEST(GameTests, startGameShouldChangeState)
{
  MockWorld world;
  MockGameObserver observer;
  EXPECT_CALL(observer, gameStateChanged).Times(1);
  Game game{world};
  EXPECT_EQ(game.getGameState(), Game::GAMESTATE_IDLE);
  game.registerGameObserver(&observer);
  std::vector<Game::SPlayer> players = {{1, 0}, {2,1},{3,2}};
  game.start(players);
}

int main(int argc, char **argv)
{
  spdlog::set_level(spdlog::level::debug);  // Set global log level to debug
  spdlog::set_pattern("[%H:%M:%S.%e %z][%l][%s][%!()][line %#] %v");
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
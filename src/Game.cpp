#include "settlers/Game.h"

#include "settlers/IGameObserver.h"

Game::Game(IWorld& world) : m_world(world)
{
}

Game::~Game()
{
}

void Game::start(
    std::vector<SPlayer> playerList)
{
  // init players

  // init world

  // init trading center

  // shuffle cards

  // build foundation turn player sequence

  // change state to foundation turns
  changeState(GAMESTATE_FOUNDATION);
}
void Game::changeState(Game::EGameState newState)
{
  if (m_gameState == newState)
  {
    return;
  }
  m_gameState = newState;
  for (auto* observer : m_gameObservers)
  {
    observer->gameStateChanged(m_gameState);
  }
}
Game::EGameState Game::getGameState() const
{
  return m_gameState;
}
void Game::registerGameObserver(IGameObserver* observer)
{
  m_gameObservers.emplace_back(observer);
}

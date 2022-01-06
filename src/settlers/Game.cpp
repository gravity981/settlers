#include <spdlog/spdlog.h>
#include "settlers/Game.h"

#include "settlers/IGameObserver.h"

Game::Game(IWorld& world) : m_world(world)
{
}

Game::~Game()
{
}

bool Game::start(
    std::vector<SPlayer> playerList,
    const std::string& worldFilePath,
    const std::string& buildingCostsFIlePath,
    const std::string& diceConfigFilePath,
    unsigned long worldSeed,
    unsigned long diceSeed)
{
  // init players
  if(playerList.size() < 2)
  {
    SPDLOG_ERROR("cannot start games with less than two players");
    return false;
  }
  for(const auto& playerData : playerList)
  {
    Player player{ playerData.number, playerData.color};
    m_players.insert(std::make_pair(player.getNumber(), player));
  }

  // init world
  if(!m_world.generateFromFile(worldFilePath, worldSeed))
  {
    return false;
  }

  // init trading center
  if(!m_tradingCenter.initFromFile(buildingCostsFIlePath))
  {
    return false;
  }

  if(!m_diceRoller.initFromFile(diceConfigFilePath))
  {
    return false;
  }

  // shuffle cards
  // todo

  // build foundation turn player sequence
  std::vector<int> foundationPhasePlayerSequence;
  for(const auto& [number, player] : m_players)
  {
    foundationPhasePlayerSequence.emplace_back(number);
  }
  for(auto it = m_players.rbegin(); it != m_players.rend(); it++)
  {
    foundationPhasePlayerSequence.emplace_back(it->first);
  }
  m_activePlayerNumber = foundationPhasePlayerSequence.front();

  // change state to foundation turns
  changeState(GAMESTATE_FOUNDATION);

  for(auto* observer : m_gameObservers)
  {
    observer->activePlayerChanged(m_activePlayerNumber);
  }
  return true;
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

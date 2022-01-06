#pragma once

#include <map>
#include <string>
#include <vector>

#include "IWorld.h"
#include "Player.h"
#include "TradingCenter.h"
#include "DiceRoller.h"

class IGameObserver;

class Game
{
 public:
  using EGameState = enum EGameState { GAMESTATE_IDLE, GAMESTATE_FOUNDATION, GAMESTATE_SETTLING, GAMESTATE_END };
  using SPlayer = struct SPlayer
  {
    int number;
    Player::EPlayerColor color;
  };

 private:
  EGameState m_gameState{ GAMESTATE_IDLE };
  int m_activePlayerNumber;
  std::map<int, Player> m_players;
  std::vector<IGameObserver*> m_gameObservers;
  IWorld& m_world;
  TradingCenter m_tradingCenter;
  DiceRoller m_diceRoller{0,0,0,0};

 public:
  explicit Game(IWorld& world);
  virtual ~Game();

  bool start(
      std::vector<SPlayer> playerList,
      const std::string& worldFilePath,
      const std::string& buildingCostsFIlePath,
      const std::string& diceConfigFilePath,
      unsigned long worldSeed,
      unsigned long diceSeed);

  EGameState getGameState() const;

  void registerGameObserver(IGameObserver* observer);

 private:
  void changeState(EGameState newState);
};

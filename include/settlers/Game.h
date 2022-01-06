#pragma once

#include <string>
#include <vector>
#include "IWorld.h"

class IGameObserver;

class Game
{
 public:
  using EGameState = enum EGameState { GAMESTATE_IDLE, GAMESTATE_FOUNDATION, GAMESTATE_SETTLING, GAMESTATE_END };
  using SPlayer = struct SPlayer
  {
    int number;
    int color;
  };

 private:
  EGameState m_gameState{ GAMESTATE_IDLE };
  std::vector<IGameObserver*> m_gameObservers;
  IWorld& m_world;

 public:
  explicit Game(IWorld& world);
  virtual ~Game();

  void start(
      std::vector<SPlayer> playerList);

  EGameState getGameState() const;

  void registerGameObserver(IGameObserver* observer);

 private:
  void changeState(EGameState newState);
};

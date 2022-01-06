#pragma once

#include "Game.h"
class IGameObserver
{
 public:
  virtual ~IGameObserver()
  {
  }

  virtual void activePlayerChanged(int activePlayer) = 0;
  virtual void gameStateChanged(Game::EGameState gameState) = 0;
};

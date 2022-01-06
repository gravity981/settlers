#pragma once

#include "settlers/IGameObserver.h"

#include <gmock/gmock.h>

class MockGameObserver: public IGameObserver
{
 public:
  ~MockGameObserver() override
  {
  }

  MOCK_METHOD1(activePlayerChanged, void(int));
  MOCK_METHOD1(gameStateChanged, void(Game::EGameState));
};
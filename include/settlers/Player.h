#pragma once

#include <functional>
#include <vector>

#include "Resource.h"
#include "Road.h"
#include "Settlement.h"
class Player
{
 public:
  using EPlayerColor = enum EPlayerColor { PLAYERCOLOR_RED, PLAYERCOLOR_BLUE, PLAYERCOLOR_ORANGE, PLAYERCOLOR_WHITE };

 private:
  const int m_playerNumber;
  const EPlayerColor m_playerColor;
  Resource m_resource;
  std::vector<std::reference_wrapper<Settlement>> m_settlements;
  std::vector<std::reference_wrapper<Road>> m_roads;

 public:
  Player(int playerNumber, EPlayerColor color);
  virtual ~Player();

  int getNumber() const;
  EPlayerColor getColor() const;
};

#include "settlers/Player.h"

Player::Player(int playerNumber, EPlayerColor color) : m_playerNumber(playerNumber), m_playerColor(color)
{
}

Player::~Player()
{
}
int Player::getNumber() const
{
  return m_playerNumber;
}
Player::EPlayerColor Player::getColor() const
{
  return m_playerColor;
}

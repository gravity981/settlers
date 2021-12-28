#include "settlers/Tile.h"

Tile::Tile(int q, int r) : m_q{ q }, m_r{ r }
{
}

Tile::Tile(const Tile& tile)
{
  m_q = tile.m_q;
  m_r = tile.m_r;
}

Tile::~Tile()
{
}

int Tile::q() const
{
  return m_q;
}

int Tile::r() const
{
  return m_r;
}

int Tile::s() const
{
  return -m_q - m_r;
}
int Tile::id() const
{
  return ((m_q + m_r) * (m_q + m_r + 1) / 2) + m_r;
}

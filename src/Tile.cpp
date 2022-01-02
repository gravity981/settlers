#include "settlers/Tile.h"

#include <spdlog/spdlog.h>

#include "settlers/Corner.h"
#include "settlers/Edge.h"

Tile::Tile(int q, int r) : m_q{ q }, m_r{ r }
{
}

Tile::Tile(const Tile& tile) = default;

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
  // collisions will start to appear when r >= MAX
  if (m_r >= Tile::MAX)
  {
    SPDLOG_WARN("Tile ID collisions may occur");
  }
  return m_q * Tile::MAX + m_r;
}

std::vector<Tile> Tile::getAllPossibleNeighbors() const
{
  // a tile has 6 neighbors
  return {
    { m_q, m_r - 1 }, { m_q + 1, m_r - 1 }, { m_q + 1, m_r }, { m_q, m_r + 1 }, { m_q - 1, m_r + 1 }, { m_q - 1, m_r },
  };
}
std::vector<Corner> Tile::getAllPossibleCorners() const
{
  auto q = static_cast<double>(m_q);
  auto r = static_cast<double>(m_r);
  // a tile has 6 corners
  return { { q + 1.0 / 3.0, r - 2.0 / 3.0 }, { q + 2.0 / 3.0, r - 1.0 / 3.0 }, { q + 1.0 / 3.0, r + 1.0 / 3.0 },
           { q - 1.0 / 3.0, r + 2.0 / 3.0 }, { q - 2.0 / 3.0, r + 1.0 / 3.0 }, { q - 1.0 / 3.0, r - 1.0 / 3.0 } };
}
std::vector<std::reference_wrapper<Tile>>& Tile::getNeighbors()
{
  return m_neighbors;
}

std::vector<std::reference_wrapper<Corner>>& Tile::getCorners()
{
  return m_corners;
}

void Tile::addNeighbor(Tile& tile)
{
  m_neighbors.emplace_back(tile);
}

void Tile::addCorner(Corner& corner)
{
  m_corners.emplace_back(corner);
}

bool Tile::allNeighborsExist() const
{
  // a hex tile can be surrounded by 6 neighbors at max
  return m_neighbors.size() == 6;
}

void Tile::setTileObject(ITileObject* tileObject)
{
  if (tileObject == nullptr)
  {
    return;
  }
  m_tileObject = tileObject;
}

ITileObject* Tile::getTileObject()
{
  return m_tileObject;
}

void Tile::addEdge(Edge& edge)
{
  m_edges.emplace_back(edge);
}

std::vector<std::reference_wrapper<Edge>>& Tile::getEdges()
{
  return m_edges;
}
std::vector<Edge> Tile::getAllPossibleEdges() const
{
  auto corners = getAllPossibleCorners();
  std::vector<Edge> edges;
  for (size_t i = 0; i < 6; i++)
  {
    auto k = (i + 1) % 6;
    Edge edge{corners[i].q(), corners[i].r(), corners[k].q(), corners[k].r()};
    edges.emplace_back(edge);
  }
  return edges;
}

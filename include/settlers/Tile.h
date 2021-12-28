#pragma once

class Tile
{
 private:
  int m_q;
  int m_r;

 public:
  explicit Tile(int q, int r);
  Tile(const Tile &tile);
  virtual ~Tile();

  int q() const;
  int r() const;
  int s() const;
  int id() const;
};

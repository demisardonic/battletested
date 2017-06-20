#ifndef TILE_H
#define TILE_H

#include <stdint.h>

class Tile {
private:
  uint8_t val;

public:
  Tile ();
  Tile (uint8_t val);
  virtual ~Tile ();
};

#endif

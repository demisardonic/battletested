#include <stdint.h>

#include "map/tile.h"

Tile::Tile(){
  this->val = 0;
}

Tile::Tile(uint8_t val){
  this->val = val;
}

Tile::~Tile(){
  
}

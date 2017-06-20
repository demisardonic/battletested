#include <string>

#include "map/map.h"
#include "map/tile.h"
#include "util.h"

Map::Map(){
  this->width = GAME_WIDTH;
  this->height = GAME_HEIGHT;
  this->floors = 1;
  this->hasLoaded = true;
  this->mapData = new Tile*[this->width * this->height * this->floors];
  for (size_t i = 0; i < this->getSize(); i++) {
    this->mapData[i] = new Tile;
  }
}

Map::Map(std::string path){
  this->hasLoaded = false;
}

Map::~Map(){
  if(this->hasLoaded){
    for (size_t i = 0; i < this->getSize(); i++) {
      delete this->mapData[i];
    }
    delete [] this->mapData;
  }
}

uint32_t Map::getTileIndex(uint16_t x, uint16_t y, uint8_t z){
  return (z * getWidth() * getHeight()) + (y * getWidth()) + x;
}

Tile *Map::getTile(uint16_t x, uint16_t y, uint16_t z){
  if(this->hasLoaded){
    uint32_t index = getTileIndex(x, y, z);
    if(index < this->getSize()){
      return this->mapData[index];
    }
  }
  return NULL;
}

void Map::setTile(Tile *tile, uint16_t x, uint16_t y, uint16_t z){
  if(this->hasLoaded){
      uint32_t index = getTileIndex(x, y, z);
      this->mapData[index] = tile;
  }
}

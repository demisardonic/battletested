#include <string>

#include "map/map.h"
#include "map/tile.h"
#include "util.h"

Map::Map(){
  this->hasLoaded = false;
}

Map::Map(std::string path){
  this->hasLoaded = false;
}

Map::~Map(){
  if(this->hasLoaded){
    delete [] this->mapData;
  }
}

uint32_t Map::getTileIndex(uint16_t x, uint16_t y, uint8_t z){
  return (z * getWidth() * getHeight()) + (y * getWidth()) + x;
}

Tile Map::getTile(uint16_t x, uint16_t y, uint16_t z){
  if(this->hasLoaded){
    uint32_t index = getTileIndex(x, y, z);
    if(index < this->getSize()){
      return this->mapData[index];
    }
  }
  return Tile(0);
}

void Map::setTile(Tile tile, uint16_t x, uint16_t y, uint16_t z){
  if(this->hasLoaded){
      uint32_t index = getTileIndex(x, y, z);
      this->mapData[index] = Tile(tile.getVal());
  }
}

Map::MapBuilder::MapBuilder(){
  this->map = new Map();
}

Map::MapBuilder& Map::MapBuilder::setSize(uint32_t size){
  map->size = size;
  map->width = 0xfff & size;
  map->height = 0xfff & (size>>12);
  map->floors = 0xff & (size>>24);
  return *this;
}

Map::MapBuilder& Map::MapBuilder::setMapData(Tile *mapData){
  map->mapData = mapData;
  return *this;
}

Map* Map::MapBuilder::build(){
  map->hasLoaded = true;
  return map;
}

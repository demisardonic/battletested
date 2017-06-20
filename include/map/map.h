#ifndef MAP_H
#define MAP_H

#include <string>

#include <stdint.h>

#include "map/tile.h"

class Map {
private:
  uint32_t size;
  uint16_t width;
  uint16_t height;
  uint8_t floors;
  bool hasLoaded;
  Tile **mapData;

public:
  Map ();
  Map (std::string path);
  virtual ~Map ();

  inline size_t getSize(){ return getWidth() * getHeight() * getFloors(); };
  inline size_t getWidth(){ return 0xfff & size; };
  inline size_t getHeight(){ return 0xfff & (size>>3); };
  inline size_t getFloors(){ return 0xff & (size>>6); };
  uint32_t getTileIndex(uint16_t x, uint16_t y, uint8_t z);
  Tile *getTile(uint16_t x, uint16_t y, uint16_t z);
  void setTile(Tile *tile, uint16_t x, uint16_t y, uint16_t z);
};

#endif

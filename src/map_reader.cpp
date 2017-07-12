#include <fstream>
#include <iostream>
#include <string>

#include <stdint.h>

#include "logger.h"
#include "map_reader.h"
#include "map/map.h"

Map* read_map_file(const std::string &path){
  std::ifstream file(path.c_str(), std::ifstream::in | std::ifstream::binary);
  uint32_t size;

  if (!file.is_open()) {
    logger("Failed to read map file %s.", path.c_str());
    return NULL;
  }

  file.read(reinterpret_cast<char *>(&size), sizeof(size));
  logger("Reading size %d", size);
  file.close();

  Map::MapBuilder builder;
  return builder.build();
}

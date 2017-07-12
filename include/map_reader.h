#ifndef MAP_READER_H
#define MAP_READER_H

#include <string>
#include "map/map.h"

Map* read_map_file(const std::string &path);

#endif

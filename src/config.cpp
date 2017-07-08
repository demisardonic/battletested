#include <fstream>
#include <sstream>
#include <string>

#include "config.h"
#include "logger.h"
#include "util.h"

Config::Config(const char* path){
  this->location = path;

  std::ifstream file(path);
  std::string buff;
  while(std::getline(file, buff)){
    std::istringstream line(buff);
    std::string key;

    if (std::getline(line, key, '=')) {
      std::string val;
      if(std::getline(line, val)){
        trim(val);
        try{
          int i = std::stoi(val);
          integers[key] = i;
          logger("Read int %s : %d", key.c_str(), i);
        }catch(...){
          if(val == "true"){
            booleans[key] = true;
            logger("Read boolean %s : true", key.c_str());
          }else if(val == "false"){
            booleans[key] = false;
            logger("Read boolean %s : false", key.c_str());
          }else{
            strings[key] = val;
            logger("Read string %s : %s", key.c_str(), val.c_str());
          }
        }
      }
    }
  }
}

Config::~Config(){}

int Config::get_int(std::string name){
  return this->integers.at(name);
}

std::string Config::get_string(std::string name){
    return this->strings.at(name);
}

bool Config::get_bool(std::string name){
  return this->booleans.at(name);
}

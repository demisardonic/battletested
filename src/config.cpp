#include <fstream>
#include <sstream>
#include <string>

#include "config.h"
#include "logger.h"
#include "util.h"

Config::Config(){
  this->location = "battle.cfg";

  logger("Initializing config %s", this->location.c_str());
  std::ifstream file(this->location);
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
        }catch(...){
          if(val == "true"){
            booleans[key] = true;
          }else if(val == "false"){
            booleans[key] = false;
          }else{
            strings[key] = val;
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

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

  std::string trueString("true");
  std::string falseString("false");
  while(std::getline(file, buff)){
    std::istringstream line(buff);
    std::string key;

    if (std::getline(line, key, '=')) {
      std::string val;
      if(std::getline(line, val)){
        std::string newVal = trim(val);
        try{
          int i = std::stoi(val);
          integers[key] = i;
        }catch(...){
          if(val == trueString){
            booleans[key] = true;
          }else if(val == falseString){
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

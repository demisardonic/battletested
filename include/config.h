#ifndef CONFIG_H
#define CONFIG_H

#include <unordered_map>
#include <string>

class Config {
private:
  std::string location;
  std::unordered_map<std::string, int> integers;
  std::unordered_map<std::string, std::string> strings;
  std::unordered_map<std::string, bool> booleans;

  Config ();
  ~Config ();


public:
  static Config& getInstance(){
    static Config config;
    return config;
  }

  int get_int(std::string name);
  std::string get_string(std::string name);
  bool get_bool(std::string name);
};

#endif

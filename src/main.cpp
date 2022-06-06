#include "Config.hpp"
#include "ConfigParser.hpp"
#include <iostream>

int main(int argc, char **argv) {
  std::string file_path;

  // Config config;
  if (argc == 2)
    file_path = argv[1];
  else
    file_path = "./config/default.conf";

  try {
    ConfigParser::parseConfigFile(file_path);
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}

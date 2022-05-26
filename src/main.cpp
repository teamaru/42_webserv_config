#include "Config.hpp"
#include "ConfigParser.hpp"
#include <iostream>

int main()
{
  ConfigParser config_parser;

  try {
    config_parser.parseConfigFile();
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}

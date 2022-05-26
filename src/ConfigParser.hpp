#ifndef CONFIG_PARSER_HPP
# define CONFIG_PARSER_HPP

#include <string>
#include <vector>
#include <fstream>
#include "Config.hpp"
#include "ConfigValidator.hpp"

#define DEFAULT_CONF_PATH "./config/default.conf"

class ConfigParser {
public:
  ConfigParser();
  ConfigParser(const ConfigParser &src);
  ~ConfigParser();
  ConfigParser &operator=(const ConfigParser &src);
  void parseConfigFile(const std::string confPath = DEFAULT_CONF_PATH);
private:
  typedef std::string::size_type size_pos;
  std::vector<std::string> tokenize(std::ifstream &ifs);
  void storeToken(std::vector<std::string> &tokens, const std::string line, size_pos pos[2]);
  void storeDelimiter(std::vector<std::string> &tokens, const std::string line, size_pos pos[2]);

  bool validateConfigFile(const std::vector<std::string> &tokens);
  void printTokens(const std::vector<std::string> &tokens);
};

#endif

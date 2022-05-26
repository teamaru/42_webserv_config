#include "ConfigParser.hpp"

ConfigParser::ConfigParser() {
}

ConfigParser::ConfigParser(const ConfigParser &src)  {
  *this = src;
}

ConfigParser::~ConfigParser() {

}

ConfigParser &ConfigParser::operator=(const ConfigParser &src) {
  if (this != &src) {
    (void)src;
  }
  return (*this);
}

void ConfigParser::storeToken(std::vector<std::string> &tokens, const std::string line, size_pos pos[2]) {
  std::string	token = line.substr(pos[BEGIN], pos[END] - pos[BEGIN]);
  token.erase(0, token.find_first_not_of(" "));
  token.erase(token.find_last_not_of(" ") + 1);
  pos[BEGIN] = pos[END];
  if (token.size() > 0)
    tokens.push_back(token);
}

void ConfigParser::storeDelimiter(std::vector<std::string> &tokens, const std::string line, size_pos pos[2]) {
  if (pos[BEGIN] == std::string::npos)
    return ;
  while (Config::DELIMITERS.find(line[pos[BEGIN]]) != std::string::npos) {
    pos[END]++;
    this->storeToken(tokens, line, pos);
  }
}

std::vector<std::string> ConfigParser::tokenize(std::ifstream &ifs) {
  std::string line;
  std::vector<std::string> tokens;
  std::string::size_type pos[2];

  while (std::getline(ifs, line)) {
    pos[BEGIN] = 0;
    pos[END] = 0;
    while (pos[BEGIN] != std::string::npos) {
      pos[END] = line.find_first_of(Config::DELIMITERS, pos[BEGIN]);
      this->storeToken(tokens, line, pos);
      this->storeDelimiter(tokens, line, pos);
    }
  }
  return (tokens);
}

void ConfigParser::parseConfigFile(const std::string confPath) {
  std::ifstream ifs(confPath);
  std::vector<std::string> tokens;

  if (!ifs)
    return ; // TODO:エラー処理
  tokens = this->tokenize(ifs);
  // printTokens(tokens);
  try {
    ConfigValidator::validateConfigFile(tokens);
  } catch (...) {
    throw ;
  }
  ifs.close();
}




void ConfigParser::printTokens(const std::vector<std::string> &tokens) {
  size_t i = -1;
  while (++i < tokens.size())
    std::cout << tokens[i] << std::endl;

}

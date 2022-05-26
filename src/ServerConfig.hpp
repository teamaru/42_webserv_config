#ifndef SERVER_CONFIG_HPP
#define SERVER_CONFIG_HPP

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

class ServerConfig {
public:
  ServerConfig();
  ServerConfig(const ServerConfig &src);
  ~ServerConfig();
  ServerConfig &operator=(const ServerConfig &src);

  void setListen(int listen);
  void setServerName(std::string serverName);
  void setClientMaxBodySize(int clientMaxBodySize);

  int getListen();
  std::string getServerName();
  int getClientMaxBodySize();
private:
  int listen_;
  std::string serverName_;
  int clientMaxBodySize_;
};

#endif

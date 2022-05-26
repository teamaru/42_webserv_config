#include "ServerConfig.hpp"

ServerConfig::ServerConfig() {
}

ServerConfig::ServerConfig(const ServerConfig &src)  {
  *this = src;
}

ServerConfig::~ServerConfig() {

}

ServerConfig &ServerConfig::operator=(const ServerConfig &src) {
  if (this != &src) {
    (void)src;
  }
  return (*this);
}

void ServerConfig::setListen(int listen) {
  this->listen_ = listen;
}

void ServerConfig::setServerName(std::string serverName) {
  this->serverName_ = serverName;
}

void ServerConfig::setClientMaxBodySize(int clientMaxBodySize) {
  this->clientMaxBodySize_ = clientMaxBodySize;
}

int ServerConfig::getListen() {
  return (this->listen_);
}

std::string ServerConfig::getServerName() {
  return (this->serverName_);
}

int ServerConfig::getClientMaxBodySize() {
  return (this->clientMaxBodySize_);
}

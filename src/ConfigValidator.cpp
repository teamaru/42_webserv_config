#include "ConfigValidator.hpp"

ConfigValidator::ConfigValidator() {
}

ConfigValidator::ConfigValidator(const ConfigValidator &src)  {
  *this = src;
}

ConfigValidator::~ConfigValidator() {

}

ConfigValidator &ConfigValidator::operator=(const ConfigValidator &src) {
  if (this != &src) {
    (void)src;
  }
  return (*this);
}

void ConfigValidator::validateConfigFile(const std::vector<std::string> tokens) {
  if (tokens.size() == 0)
    return ;
  if (!isValidBraceNum(tokens))
    throw (std::runtime_error(ERR_MSG_IVLD_NUM_BRCKT));
  if (!isValidBracePlace(tokens, DRCTV_NAME_SRVR))
    throw (std::runtime_error(ERR_MSG_UNEXPCTD_TKN));
  if (!isValidBracePlace(tokens, DRCTV_NAME_LCTN))
    throw (std::runtime_error(ERR_MSG_UNEXPCTD_TKN));

  try {
    checkMainDirectives(tokens);
    checkDerective(tokens, DRCTV_NAME_SRVR);
    checkDerective(tokens, DRCTV_NAME_LCTN);
    checkAllDirectives(tokens);
  } catch (...) {
    throw ;
  }
}

bool ConfigValidator::isValidBraceNum(const std::vector<std::string> tokens) {
  int open_brace_num = std::count(tokens.begin(), tokens.end(), "{");
  int close_brace_num = std::count(tokens.begin(), tokens.end(), "}");
  return (open_brace_num == close_brace_num);
}

bool ConfigValidator::isValidBracePlace(const std::vector<std::string> tokens, std::string target) {
  int pad = target == DRCTV_NAME_SRVR ? 1 : 2;
  str_vec_itr it = tokens.begin();
  while ((it = std::find(it, tokens.end(), target)) != tokens.end()) {
    it += pad;
    if (*it != "{")
      return (false);
  }
  return (true);
}

bool ConfigValidator::isServerExist(const std::vector<std::string> tokens) {
  return (std::find(tokens.begin(), tokens.end(), DRCTV_NAME_SRVR) != tokens.end());
}

void ConfigValidator::checkDerective(const std::vector<std::string> tokens, const std::string target) {
  str_vec_itr it[2];
  it[BEGIN] = std::find(tokens.begin(), tokens.end(), target);

  while (it[BEGIN] != tokens.end()) {
    it[BEGIN] = std::find(it[BEGIN], tokens.end(), "{");
    it[END] = it[BEGIN];
    findEndBrace(++it[END]);
    try {
      scanDerective(it, target);
    } catch (...) {
      throw ;
    }
    it[BEGIN] = std::find(it[BEGIN], tokens.end(), target);
  }
}

void ConfigValidator::findEndBrace(str_vec_itr &it) {
  while (*it != "}") {
    if (*it == "{")
      findEndBrace(++it);
    it++;
  }
}

void ConfigValidator::scanDerective(str_vec_itr it[2], const std::string target) {
  while (++it[BEGIN] != it[END]) {
    if (*it[BEGIN] == "{")
      findEndBrace(++it[BEGIN]);
    if (!isValidDirectiveName(it[BEGIN], target))
      throw (std::runtime_error(ERR_MSG_INVLD_DRCTV));
    if (isDerectiveDuplicated(it[BEGIN], it[END]))
      throw (std::runtime_error(ERR_MSG_DPLCT_DRCTV));
    if (!isValidValueNum(it[BEGIN]))
      throw (std::runtime_error(ERR_MSG_INVLD_VALUE_NUM));
    if (!isLocationDuplicated(it[BEGIN], it[END]))
      throw (std::runtime_error(ERR_MSG_DPLCTD_LCTN));
  }
}

bool ConfigValidator::isDerectiveDuplicated(str_vec_itr begin, str_vec_itr end) {
  std::string target = *begin;
  if (!isDirective(begin) || target == DRCTV_NAME_LCTN || target == DRCTV_NAME_ERR_PG)
    return (false);
  while (++begin != end) {
    if (!isDirective(begin))
      continue ;
    if (target == *begin)
      return (true);
  }
  return (false);
}


bool ConfigValidator::isDirective(str_vec_itr it) {
  return (Config::DELIMITERS.find(*(it - 1)) != std::string::npos && Config::DELIMITERS.find(*it) == std::string::npos);
}

e_drctv_cd ConfigValidator::getDirectiveCode(std::string target) {
  str_vec_itr it = Config::DERECTIVE_NAMES.begin();
  size_t directive_code = 0;
  while (it != Config::DERECTIVE_NAMES.end() && *it != target) {
    directive_code++;
    it++;
  }
  return ((e_drctv_cd)directive_code);
}

bool ConfigValidator::isValidDirectiveName(str_vec_itr it, std::string target) {
  if (!isDirective(it))
    return (true);
  std::vector<std::string> dirs = Config::DERECTIVE_MAP.at(getDirectiveCode(target));
  return (std::find(dirs.begin(), dirs.end(), *it) != dirs.end());
}

void ConfigValidator::checkMainDirectives(const std::vector<std::string> tokens) {
  str_vec_itr it[2];
  it[BEGIN] = tokens.begin();
  it[END] = tokens.end();
  while (it[BEGIN] != it[END]) {
    if (*it[BEGIN] == "{")
      findEndBrace(++it[BEGIN]);
    if (*it[BEGIN] != "}" && *it[BEGIN] != DRCTV_NAME_SRVR)
      throw (std::runtime_error(ERR_MSG_INVLD_DRCTV));
    it[BEGIN]++;
  }
}

bool ConfigValidator::isValidValueNum(str_vec_itr it) {
  size_t cnt = 0;
  std::string directive;

  if (!isDirective(it))
    return (true);
  directive = *it++;
  while (Config::DELIMITERS.find(*it++) == std::string::npos)
    cnt++;
  if (cnt == 0)
    return (false);
  if (cnt == 2 && directive != DRCTV_NAME_ERR_PG && directive != DRCTV_NAME_ALLWD_MTHD)
    return (false);
  if (directive == DRCTV_NAME_ERR_PG && cnt != 2)
    return (false);
  if (cnt == 3 && directive != DRCTV_NAME_ALLWD_MTHD)
    return (false);
  if (cnt >= 4)
    return (false);
  return (true);
}

void ConfigValidator::checkAllDirectives(const std::vector<std::string> tokens) {
  str_vec_itr it[2];
  it[BEGIN] = tokens.begin();
  it[END] = tokens.end();
  try {
    while (it[BEGIN] != it[END]) {
      validatePort(it[BEGIN], it[END]);
      it[BEGIN]++;
    }
  } catch (...) {
    throw ;
  }
}

void ConfigValidator::validatePort(str_vec_itr begin, str_vec_itr end) {
  if (*begin != DRCTV_NAME_LSTN)
    return ;
  try {
    int port = stoi(*++begin);
    while (++begin != end) {
      if (!isDirective(begin) || *begin != DRCTV_NAME_LSTN)
        continue ;
      if (port == stoi(*++begin))
        throw (std::runtime_error(ERR_MSG_DPLCTD_PORT));
    }
  } catch (std::runtime_error &e) {
    throw (e);
  } catch (...) {
    throw (std::runtime_error(ERR_MSG_INVLD_PORT));
  }
}

bool ConfigValidator::isLocationDuplicated(str_vec_itr begin, str_vec_itr end) {
  if (*begin != DRCTV_NAME_LCTN)
    return (true);
  std::string target = *++begin;
  while (++begin != end) {
    if (!isDirective(begin) || *begin != DRCTV_NAME_LCTN)
      continue ;
    if (target == *++begin)
      return (false);
  }
  return (true);
}

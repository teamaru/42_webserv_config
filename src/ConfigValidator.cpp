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

bool ConfigValidator::checkDerective(const std::vector<std::string> tokens, const std::string target) {
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
  return (true);
}

void ConfigValidator::findEndBrace(str_vec_itr &it) {
  while (*it != "}") {
    if (*it == "{")
      findEndBrace(++it);
    it++;
  }
}

bool ConfigValidator::searchToken(str_vec_itr it[2], const std::string target) {
  while (++it[BEGIN] != it[END]) {
    if (*it[BEGIN] == "{")
      findEndBrace(++it[BEGIN]);
    if (*it[BEGIN] == target)
      return (true);
  }
  return (false);
}

bool ConfigValidator::scanDerective(str_vec_itr it[2], const std::string target) {
  std::map<const std::string, int> directives = createDuplicateCheckMap(target);
  while (++it[BEGIN] != it[END]) {
    if (*it[BEGIN] == "{")
      findEndBrace(++it[BEGIN]);
    if (!isValidDirectiveName(it[BEGIN], target))
      throw (std::runtime_error(ERR_MSG_INVLD_DRCTV));
    if (isDerectiveDuplicated(it[BEGIN], directives))
      throw (std::runtime_error(ERR_MSG_DPLCT_DRCTV));
  }
  return (false);
}

bool ConfigValidator::isDerectiveDuplicated(str_vec_itr it, std::map<const std::string, int> &directives) {
  if (!isDirective(it))
    return (false);
  std::map<const std::string, int>::iterator directive = directives.begin();
  while (directive != directives.end()) {
    if (*it == directive->first) {
      directive->second++;
      if (directive->second >= 2)
        return (true);
    }
    directive++;
  }
  return (false);
}

bool ConfigValidator::isDirective(str_vec_itr it) {
  return (Config::DELIMITERS.find(*(it - 1)) != std::string::npos && Config::DELIMITERS.find(*it) == std::string::npos);
}

std::map<const std::string, int> ConfigValidator::createDuplicateCheckMap(const std::string target) {
  std::map<const std::string, int> map;

  if (target == DRCTV_NAME_SRVR) {
    map[DRCTV_NAME_LSTN] = 0;
    map[DRCTV_NAME_SRVR_NM] = 0;
    map[DRCTV_NAME_MX_CLNT_BDY_SZ] = 0;
  } else if (target == DRCTV_NAME_LCTN) {
    map[DRCTV_NAME_ALLWD_MTHD] = 0;
    map[DRCTV_NAME_ROOT] = 0;
    map[DRCTV_NAME_OUT_INDX] = 0;
    map[DRCTV_NAME_INDX] = 0;
    map[DRCTV_NAME_RDRCT] = 0;
  }
  return (map);
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
  while (it[BEGIN] != tokens.end()) {
    if (*it[BEGIN] == "{")
      findEndBrace(++it[BEGIN]);
    if (!isValidDirectiveName(it[BEGIN], DRCTV_NAME_MAIN))
      throw (std::runtime_error(ERR_MSG_INVLD_DRCTV));
    it[BEGIN]++;
  }
}

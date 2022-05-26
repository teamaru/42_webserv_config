#include "Config.hpp"

const std::string Config::DELIMITERS = " {};";
const std::vector<std::string> Config::DERECTIVE_NAMES = Config::createDerectiveNameVec();
const std::map<const e_drctv_cd, std::vector<std::string> > Config::DERECTIVE_MAP = Config::createDerectiveMap();

Config::Config() {
}

Config::Config(const Config &src)  {
  *this = src;
}

Config::~Config() {
}

Config &Config::operator=(const Config &src) {
  if (this != &src) {
    (void)src;
  }
  return (*this);
}

const std::vector<std::string> Config::createDerectiveNameVec() {
  std::vector<std::string> derective_names;

  derective_names.push_back(DRCTV_NAME_SRVR);
  derective_names.push_back(DRCTV_NAME_LCTN);
  derective_names.push_back(DRCTV_NAME_LSTN);
  derective_names.push_back(DRCTV_NAME_SRVR_NM);
  derective_names.push_back(DRCTV_NAME_MX_CLNT_BDY_SZ);
  derective_names.push_back(DRCTV_NAME_ALLWD_MTHD);
  derective_names.push_back(DRCTV_NAME_ROOT);
  derective_names.push_back(DRCTV_NAME_OUT_INDX);
  derective_names.push_back(DRCTV_NAME_INDX);
  derective_names.push_back(DRCTV_NAME_RDRCT);
  derective_names.push_back(DRCTV_NAME_ERR_PG);
  return (derective_names);
}

const std::map<const e_drctv_cd, std::vector<std::string> > Config::createDerectiveMap() {
  std::map<const e_drctv_cd, std::vector<std::string> > derective_map;
  std::vector<std::string> main_dirs;
  std::vector<std::string> server_dirs;
  std::vector<std::string> location_dirs;

  main_dirs.push_back(DRCTV_NAME_SRVR);
  server_dirs.push_back(DRCTV_NAME_LCTN);
  server_dirs.push_back(DRCTV_NAME_LSTN);
  server_dirs.push_back(DRCTV_NAME_SRVR_NM);
  server_dirs.push_back(DRCTV_NAME_MX_CLNT_BDY_SZ);
  server_dirs.push_back(DRCTV_NAME_ERR_PG);
  location_dirs.push_back(DRCTV_NAME_ALLWD_MTHD);
  location_dirs.push_back(DRCTV_NAME_ROOT);
  location_dirs.push_back(DRCTV_NAME_OUT_INDX);
  location_dirs.push_back(DRCTV_NAME_INDX);
  location_dirs.push_back(DRCTV_NAME_RDRCT);
  derective_map[MAIN] = main_dirs;
  derective_map[SRVR] = server_dirs;
  derective_map[LCTN] = location_dirs;
  // std::map<const e_drctv_cd, std::vector<std::string> >::iterator directive = derective_map.begin();
  // while (directive != derective_map.end()) {
  //   std::cout << "-------- " << directive->first << " --------" << std::endl;
  //   std::vector<std::string>::iterator it = directive->second.begin();
  //   while (it != directive->second.end()) {
  //     std::cout << *it << std::endl;
  //     it++;
  //   }
  //   directive++;
  // }
  return (derective_map);
}

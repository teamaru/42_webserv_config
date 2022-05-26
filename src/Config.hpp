#ifndef CONFIG_HPP
# define CONFIG_HPP

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include "ServerConfig.hpp"

#define DEFAULT_CONF_PATH "./config/default.conf"

#define DRCTV_NAME_SRVR "server"
#define DRCTV_NAME_LCTN "location"
#define DRCTV_NAME_LSTN "listen"
#define DRCTV_NAME_SRVR_NM "server_name"
#define DRCTV_NAME_MX_CLNT_BDY_SZ "max_client_body_size"
#define DRCTV_NAME_ALLWD_MTHD "allowed_method"
#define DRCTV_NAME_ROOT "root"
#define DRCTV_NAME_OUT_INDX "auto_index"
#define DRCTV_NAME_INDX "index"
#define DRCTV_NAME_RDRCT "return"
#define DRCTV_NAME_ERR_PG "error_page"
#define DRCTV_NAME_MAIN "main"

enum e_drctv_cd {
  SRVR,
  LCTN,
  LSTN,
  SRVR_NM,
  MX_CLNT_BDY_SZ,
  ALLWD_MTHD,
  ROOT,
  AOUT_INDX,
  INDX,
  RDRCT,
  ERR_PG,
  MAIN,
};

enum e_pos_type {
  BEGIN,
  END,
};

typedef std::vector<const std::string> str_vec;
typedef str_vec::iterator str_vec_itr;

class Config {
public:
  Config();
  Config(const Config &src);
  ~Config();
  Config &operator=(const Config &src);

  static const std::string DELIMITERS;
  static const std::vector<std::string> DERECTIVE_NAMES;
  static const std::map<const e_drctv_cd, std::vector<std::string> > DERECTIVE_MAP;
private:
  std::vector<ServerConfig> serverConfigs_;
  static const std::vector<std::string> createDerectiveNameVec();
  static const std::map<const e_drctv_cd, std::vector<std::string> > createDerectiveMap();
};

#endif

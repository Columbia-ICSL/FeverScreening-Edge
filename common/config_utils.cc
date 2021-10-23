//
// Copyright 2020 ICSL. All Rights Reserved.
//

#include "common/config_utils.h"

namespace common {
namespace config {
nlohmann::json LoadJsonConfigFromFile(char* json_file) {
  std::ifstream i(json_file);
  nlohmann::json j;
  i >> j;
  return j;
}

std::unordered_map<std::string, std::string> GetSystemMacAddressMap() {
  std::unordered_map<std::string, std::string> dummy;
  return dummy;
}

std::string GetEnvVariable(const char* env, const std::string backup) {
  return GetEnvVariable(env, backup.c_str());
}

std::string GetEnvVariable(const char* env, const char* backup) {
  std::string result;
  const char* var = std::getenv(env);
  if (var != NULL) {
    result.assign(var);
  } else {
    result.assign(backup);
  }
  return result;
}
}  // namespace config
}  // namespace common
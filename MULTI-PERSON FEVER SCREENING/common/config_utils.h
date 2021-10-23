//
// Copyright 2020 ICSL. All Rights Reserved.
//

#ifndef COMMON_CONFIG_UTILS_H_
#define COMMON_CONFIG_UTILS_H_

#include <cstdlib>
#include <fstream>
#include <unordered_map>

#include <dirent.h>
#include <sys/types.h>

#include "nlohmann/json.hpp"

namespace common {
namespace config {
nlohmann::json LoadJsonConfigFromFile(char* json_file);
std::unordered_map<std::string, std::string> GetSystemMacAddressMap();

std::string GetEnvVariable(const char* env, const std::string backup);
std::string GetEnvVariable(const char* env, const char* backup);
}  // config
}  // common
#endif  // COMMON_CONFIG_UTILS_H_
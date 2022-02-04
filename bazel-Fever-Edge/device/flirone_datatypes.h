//
// Copyright 2020 ICSL. All Rights Reserved.
//

#ifndef DEVICE_FLIRONE_DATATYPES_H_
#define DEVICE_FLIRONE_DATATYPES_H_

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <vector>

#include <glog/logging.h>

#include "nlohmann/json.hpp"
#include "common/base64.h"
#include "common/encryption_utils.h"
#include "device/flirone_calibration_utils.h"

namespace device {
// private constant
constexpr int32_t kThermalWidth = 160;
constexpr int32_t kThermalHeight = 120;
constexpr int32_t kThermalChannels = 1;
constexpr int32_t kThermalDepth = 16;
constexpr int32_t kRGBWidth = 640;
constexpr int32_t kRGBHeight = 480;
constexpr int32_t kRGBChannels = 3;
constexpr int32_t kRGBDepth = 8;
constexpr int32_t kMaxStatusSize = 200;
constexpr uint32_t kEPErrorSize = 50;
constexpr uint32_t kBuf85Size = 2 * 131072;
constexpr uint32_t kBufFileSize = 1048576;
constexpr uint32_t kBufConfigSize = 4096;
constexpr uint32_t kMaxJpgSize = 200000;

class FlirOneImageResult {
 public:
  std::string machine_id = "default";
  int64_t timestamp_ns = 0;
  uint32_t sequence = 0;
  int32_t thermal_width = kThermalWidth;
  int32_t thermal_height = kThermalHeight;
  int32_t thermal_channels = kThermalChannels;
  int32_t thermal_depth = kThermalDepth;
  int32_t thermal_size = kThermalWidth * kThermalHeight * kThermalChannels;
  int32_t thermal_data_size =
      2 * kThermalWidth * kThermalHeight * kThermalChannels;
  std::string thermal_data_type = "uint16 in uint8";
  uint16_t thermal_data[
      kThermalWidth * kThermalHeight * kThermalChannels];
  std::string tempearture_data_type = "int16 in uint8, 0.01K per digit";
  int16_t temperature_data[
      kThermalWidth * kThermalHeight * kThermalChannels];
  int32_t rgb_width = kThermalWidth;
  int32_t rgb_height = kThermalHeight;
  int32_t rgb_channels = kThermalChannels;
  int32_t rgb_depth = kRGBDepth;
  uint8_t rgb_jpg[kMaxJpgSize];
  uint32_t rgb_jpg_size = 0;
  uint8_t status_data[kMaxStatusSize];
  int32_t status_size = 0;

  FlirOneImageResult() {}
  ~FlirOneImageResult();

  void SetEncryptionProfile(
      const common::encryption::EncryptionProfile& profile);
  nlohmann::json ToSlimJson() const;
  nlohmann::json ToJson() const;
  std::vector<std::uint8_t> ToBson() const;
  std::string ToJsonString() const;
  void ToJsonString(std::string* output) const;
  void ToEncryptedJsonString(std::string* output);

  void ConvertRawToTemp();
  FlirOneImageResult& operator = (const FlirOneImageResult &result);

 private:
   ::common::encryption::EncryptionUtils encrypt_utils;
};
}  // namespace device
#endif  // DEVICE_FLIRONE_DATATYPES_H_
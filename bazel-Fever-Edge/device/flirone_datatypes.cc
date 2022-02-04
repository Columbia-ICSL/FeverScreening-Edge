#include "device/flirone_datatypes.h"

#include <iostream>

namespace device {

FlirOneImageResult::~FlirOneImageResult() {
}

void FlirOneImageResult::SetEncryptionProfile(
    const common::encryption::EncryptionProfile& profile) {
  encrypt_utils.SetEncryptionProfile(profile);
}

nlohmann::json FlirOneImageResult::ToSlimJson() const {
  nlohmann::json msg;
  msg["machine_id"] = machine_id;
  msg["timestamp_ns"] = timestamp_ns;
  msg["sequence"] = sequence;
  msg["thermal_width"] = thermal_width;
  msg["thermal_height"] = thermal_height;
  msg["thermal_channels"] = thermal_channels;
  msg["thermal_size"] = thermal_size;
  msg["thermal_data_size"] = thermal_data_size;
  msg["thermal_data_type"] = thermal_data_type;
  msg["temperature_data_type"] = tempearture_data_type;
  msg["rgb_width"] = rgb_width;
  msg["rgb_height"] = rgb_height;
  msg["rgb_channels"] = rgb_channels;
  msg["rgb_jpg_size"] = rgb_jpg_size;
  msg["thermal_data"] = "This data is ignored in the slim messege.";
  msg["temperature_data"] = "This data is ignored in the slim messege.";
  msg["rgb_jpg_data"] = "This data is ignored in the slim messege.";
  return msg;
}

nlohmann::json FlirOneImageResult::ToJson() const {
  nlohmann::json msg = ToSlimJson();
  // covnert thermal_data to uint8_t
  uint8_t local_thermal_data[thermal_data_size];
  #pragma omp parallel for
  for (int16_t i = 0; i < thermal_size; i++) {
    uint16_t thermal_word = thermal_data[i];
    local_thermal_data[i * 2] = thermal_word >> 8;
    local_thermal_data[i * 2 + 1] = thermal_word & 0xff;
  }
  msg["thermal_data"] = base64_encode(local_thermal_data, thermal_data_size);
  
  // convert temperature_data to uint8_t
  #pragma omp parallel for
  for (int16_t i = 0; i < thermal_size; i++) {
    int16_t thermal_word = temperature_data[i];
    local_thermal_data[i * 2] = thermal_word >> 8;
    local_thermal_data[i * 2 + 1] = thermal_word & 0xff;
  }
  msg["temperature_data"] =
      base64_encode(local_thermal_data, thermal_data_size);
  // convert rgb_jpg_data
  msg["rgb_jpg_data"] = base64_encode(rgb_jpg, rgb_jpg_size);
  return msg;
}

std::vector<std::uint8_t> FlirOneImageResult::ToBson() const {
  return nlohmann::json::to_bson(ToJson());
}

std::string FlirOneImageResult::ToJsonString() const {
  return ToJson().dump(); 
}

void FlirOneImageResult::ToJsonString(
    std::string* output) const {
  std::string temp_output = ToJsonString();
  *output = std::move(temp_output); 
}

void FlirOneImageResult::ToEncryptedJsonString(std::string* output) {
  std::string json_str = ToJsonString();
  encrypt_utils.EncryptData(json_str, output);
}

void FlirOneImageResult::ConvertRawToTemp() {
  flir::FlirOneCalibProfile profile;
  #pragma omp parallel for
  for (uint16_t i = 0; i < thermal_size; i++) {
    temperature_data[i] = flir::FlirOneCalibUtil::ConvertRawToInt16K(
        profile, thermal_data[i]);
  }
}
}  // namespace device
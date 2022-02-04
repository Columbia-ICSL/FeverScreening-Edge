//
// Copyright 2020 ICSL. All Rights Reserved.
//

#include "gtest/gtest.h"
#include "device/flirone_interface.h"

namespace device {
TEST(FlirOneInterface, constructor_1) {
  FlirOneConfig config;
  FlirOneImageResult result;
  config.name = "Test";
  FlirOneInterface flir_interface(config);
  flir_interface.Run();
  for (uint32_t i = 0; i < 10; i++) {
    LOG(INFO) << "Try to read frame: " << i;
    EXPECT_EQ(flir_interface.PollGetResult(&result), common::Status::OK);
    EXPECT_EQ(result.sequence, i);
    EXPECT_GT(result.rgb_jpg_size, 50000);
    EXPECT_GT(result.timestamp_ns, 10000000000);
  }
  flir_interface.Stop();
}

TEST(FlirOneInterface, flirone_datatypes_1) {
  FlirOneConfig config;
  FlirOneImageResult result;
  config.name = "Test";
  FlirOneInterface flir_interface(config);
  flir_interface.Run();
  for (uint32_t i = 0; i < 5; i++) {
    LOG(INFO) << "Try to read frame: " << i;
    EXPECT_EQ(flir_interface.PollGetResult(&result), common::Status::OK);
    std::string result_str = result.ToJsonString();
    nlohmann::json result_json = nlohmann::json::parse(result_str);
    EXPECT_EQ(result_json["sequence"], i);
    EXPECT_GT(result_json["rgb_jpg_size"], 50000);
    EXPECT_GT(result_json["timestamp_ns"], 10000000000);
  }
  flir_interface.Stop();
}

TEST(FlirOneInterface, flirone_encryption_1) {
  FlirOneConfig config;
  FlirOneImageResult result;
  config.name = "Test";
  FlirOneInterface flir_interface(config);
  flir_interface.Run();
  for (uint32_t i = 0; i < 5; i++) {
    LOG(INFO) << "Try to read frame: " << i;
    EXPECT_EQ(flir_interface.PollGetResult(&result), common::Status::OK);
    std::string result_str = result.ToJsonString();
    std::string cipher_str;
    result.ToEncryptedJsonString(&cipher_str);
    EXPECT_EQ(cipher_str.length(), result_str.length());
  }
  flir_interface.Stop();
}

TEST(FlirOneInterface, flirone_temperature_1) {
  FlirOneConfig config;
  FlirOneImageResult result;
  config.name = "Test";
  FlirOneInterface flir_interface(config);
  flir_interface.Run();
  flir::FlirOneCalibProfile calib_profile;
  for (uint32_t i = 0; i < 5; i++) {
    LOG(INFO) << "Try to read frame: " << i;
    EXPECT_EQ(flir_interface.PollGetResult(&result), common::Status::OK);
    for (uint16_t i = 0; i < result.thermal_size; i++) {
      EXPECT_EQ(result.temperature_data[i],
          flir::FlirOneCalibUtil::ConvertRawToInt16C(
              calib_profile, result.thermal_data[i]));
    }
  }
  flir_interface.Stop();
}
}
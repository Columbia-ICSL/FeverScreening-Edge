#include <iostream>

#include <glog/logging.h>

#include "device/flirone_interface.h"
#include "hiredis/hiredis.h"


#define PUB_CHANNEL "flirone_pub"

void SignalHandle(const char* data, int size) {
  std::string str = data;
  LOG(ERROR) << str;
}

int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();
  google::InstallFailureWriter(&SignalHandle);

  // EncryptionProfile
  common::encryption::EncryptionProfile profile;
  profile.name = "ICSL";
  profile.algorithm = common::encryption::EncryptionAlgo::AES_CFB;

  LOG(INFO) << "Config and run flirone interface";
  device::FlirOneConfig config;
  device::FlirOneImageResult result;
  config.name = "Test";
  device::FlirOneInterface flir_interface(config);
  flir_interface.Run();

  // Setup Redis Pub/Sub
  redisContext *redis_context = redisConnect("127.0.0.1", 6379);
  if (redis_context->err) {
    printf("error: %s\n", redis_context->errstr);
    return 1;
  }

  std::string result_json;
  // Infinit loop
  while (flir_interface.PollGetResult(&result) ==
         common::Status::OK) {
    result.ToEncryptedJsonString(&result_json);
    std::cout << "Read a new frame: "
        << "timestamp_ns: " << result.timestamp_ns
        << ", sequence: " << result.sequence
        << ", rgb_jpg_size: " << result.rgb_jpg_size
        << ", thermal_size: " << result.thermal_size
        << ", json.length(): " << result_json.length() << std::endl;
    redisCommand(redis_context, "PUBLISH %s %s", PUB_CHANNEL, result_json);
  }
  flir_interface.Stop();
  return 0;
}
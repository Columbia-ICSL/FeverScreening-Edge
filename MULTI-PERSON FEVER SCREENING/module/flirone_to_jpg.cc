#include <iostream>
#include <fstream>

#include <glog/logging.h>

#include "device/flirone_interface.h"


void SignalHandle(const char* data, int size) {
  std::string str = data;
  LOG(ERROR) << str;
}

int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();
  google::InstallFailureWriter(&SignalHandle);

  if(argc <=1) {
    LOG(ERROR) << "Please specify output dir for jpeg";
  }

  std::string output_dir(argv[1]);
  device::FlirOneConfig config;
  device::FlirOneImageResult result;
  config.name = "Test";
  device::FlirOneInterface flir_interface(config);
  flir_interface.Run();

  std::string result_json;
  // Infinit loop
  while (flir_interface.PollGetResult(&result) ==
         common::Status::OK) {
    std::string file_name = output_dir + "/"
        + std::to_string(result.timestamp_ns) + ".jpg";
    std::ofstream jpg_output;
    jpg_output.open(file_name, std::ios::out | std::ios::binary);
    if (!jpg_output) {
      LOG(ERROR) << "Cannot open file " << file_name;
      continue;
    } else {
      jpg_output.write(
          (char*)result.rgb_jpg, result.rgb_jpg_size);
      jpg_output.close();
      std::cout << "Wrote to " << file_name << std::endl;
    }
  }
  flir_interface.Stop();
  return 0;
}
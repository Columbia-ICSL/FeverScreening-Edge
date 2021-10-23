#include <iostream>

#include <glog/logging.h>
#include <curl/curl.h>

#include "common/config_utils.h"
#include "device/flirone_interface.h"
#include "hiredis/hiredis.h"


#define PUB_CHANNEL "flirone_json_pub"

constexpr char kENVBindIp[] = "ENV_BIND_IP";
constexpr char kENVBindPort[] = "ENV_BIND_PORT";
constexpr char kENVBindURL[] = "ENV_BIND_URL";

void SignalHandle(const char* data, int size) {
  std::string str = data;
  LOG(ERROR) << str;
}

common::Status CreateCurlEasyHandle(CURL* curl_handle,
    const nlohmann::json& curl_config) {
  if (!curl_config.contains("server_ip")
      || !curl_config.contains("server_port")
      || !curl_config.contains("server_url")) {
    LOG(ERROR) << "Config file is incomplete";
    return common::Status::ERROR;
  }
  std::string server_ip = common::config::GetEnvVariable(
      kENVBindIp, curl_config["server_ip"].get<std::string>().c_str());
  std::string server_port = common::config::GetEnvVariable(
      kENVBindPort, curl_config["server_port"].get<std::string>().c_str());
  std::string server_url = common::config::GetEnvVariable(
      kENVBindURL, curl_config["server_url"].get<std::string>().c_str());
  std::stringstream host_ss;
  host_ss << "fever_server.com:" << server_port << ":" << server_ip;
  struct curl_slist *host = NULL;
  host = curl_slist_append(NULL, host_ss.str().c_str());
  curl_easy_setopt(curl_handle, CURLOPT_RESOLVE, host);
  curl_easy_setopt(curl_handle, CURLOPT_URL,
      std::string("fever_server.com" + server_url).c_str());
  curl_easy_setopt(curl_handle, CURLOPT_PORT, stoi(server_port));
  std::cout << "curl_handle has been set to: "
      << server_ip << ":" << server_port << std::endl;
  return common::Status::OK;
}

common::Status SendPOSTRequest(CURL* curl_handle, const std::string* flir_result) {
  struct curl_slist *headers=NULL;
  headers = curl_slist_append(headers,
      "Content-Type: Application/x-www-form-urlencoded");
  // post binary data
  curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, flir_result->data());
  // set the size of the postfields data
  curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDSIZE, flir_result->length());
  // pass our list of custom made headers
  curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, headers);
  // Post away
  int ret = curl_easy_perform(curl_handle);
  curl_slist_free_all(headers);
  if (ret == 0) {
    std::cout << "Sent out curl_post request" << std::endl;
  } else {
    std::cout << "Err while seting curl post: " << ret << std::endl;
  }
  return common::Status::OK;
}

int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();
  google::InstallFailureWriter(&SignalHandle);

  if (argc <= 1) {
    LOG(ERROR) << "Please specify the curl_server.json";
    exit(1);
  }
  // Read client_config.
  auto client_config = common::config::LoadJsonConfigFromFile(argv[1]);

  // Init curl
  curl_global_init(CURL_GLOBAL_ALL);
  CURL* curl_handle = curl_easy_init();
  CreateCurlEasyHandle(curl_handle, client_config);

  // EncryptionProfile
  common::encryption::EncryptionProfile profile;
  common::encryption::EncryptionUtils encryption_util;
  profile.name = "ICSL";
  profile.algorithm = common::encryption::EncryptionAlgo::AES_CFB;
  profile.ReadKeyIv(
      client_config["aes_key"].get<std::string>(),
      client_config["aes_iv"].get<std::string>());
  encryption_util.SetEncryptionProfile(profile);

  // Create and run FlirOneInterface
  device::FlirOneConfig config;
  device::FlirOneImageResult result;
  config.name = "Test";
  config.machine_id = client_config["machine_id"].get<std::string>();
  device::FlirOneInterface flir_interface(config);
  flir_interface.Run();
  result.SetEncryptionProfile(profile);

  std::string result_json;
  std::string result_base64;
  std::string result_reversed;
  // Infinit loop
  while (flir_interface.PollGetResult(&result) ==
         common::Status::OK) {
    result.ToEncryptedJsonString(&result_json);
    result_base64 = base64_encode(result_json);
    encryption_util.DecryptData(result_json, &result_reversed);
    std::cout << "Read a new frame: "
        << "timestamp_ns: " << result.timestamp_ns
        << ", sequence: " << result.sequence
        << ", rgb_jpg_size: " << result.rgb_jpg_size
        << ", thermal_size: " << result.thermal_size
        << ", json.length(): " << result_json.length()
        << ", encoded length(): " << result_base64.length()
        << ", Sample data: " << result_json.substr(0, 20) << std::endl;
    SendPOSTRequest(curl_handle, &result_base64);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }

  // Cleanup
  flir_interface.Stop();
  curl_global_cleanup();
  return 0;
}
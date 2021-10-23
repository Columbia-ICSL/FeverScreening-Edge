/*
  A trivial static http webserver using Libevent's evhttp.

  This is not the best code in the world, and it does some fairly stupid stuff
  that you would never want to do in a production webserver. Caveat hackor!

 */

#include <cstdlib>
#include <deque>
#include <iostream>
#include <mutex>

#include "nlohmann/json.hpp"

#include "common/base64.h"
#include "common/config_utils.h"
#include "device/flirone_datatypes.h"
#include "device/http_post_server_interface.h"
#include "hiredis/hiredis.h"

#define PUB_CHANNEL "flirone_json_pub"

constexpr int kDefaultWaitNextLoopTimeMS = 100;
constexpr int kMaxDequeBufferSize = 2;
constexpr char kRedisIp[] = "127.0.0.1";
constexpr char kRedisPort[] = "6379";
constexpr char kENVBindIp[] = "ENV_BIND_IP";
constexpr char kENVBindPort[] = "ENV_BIND_PORT";
constexpr char kENVBindURL[] = "ENV_BIND_URL";
constexpr char kENVPubChannel[] = "ENV_FLIR_JSON_PUB";
constexpr char kENVRedisIp[] = "ENV_FLIR_JSON_PUB";
constexpr char kENVRedisPort[] = "ENV_FLIR_JSON_PUB";

void SignalHandle(const char* data, int size) {
  std::string str = data;
  LOG(ERROR) << str;
}

int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();
  google::InstallFailureWriter(&SignalHandle);

  // Read config
  if (argc <= 1) {
    LOG(ERROR) << "Please specify the curl_server.json";
    exit(1);
  }
  auto server_config =
      common::config::LoadJsonConfigFromFile(argv[1]);

  // EncryptionProfile
  common::encryption::EncryptionProfile profile;
  common::encryption::EncryptionUtils encryption_util;
  profile.name = "ICSL";
  profile.algorithm = common::encryption::EncryptionAlgo::AES_CFB;
  profile.ReadKeyIv(
      server_config["aes_key"].get<std::string>(),
      server_config["aes_iv"].get<std::string>());
  encryption_util.SetEncryptionProfile(profile);

  // Post data deque
  std::deque<std::string> post_data_deque;
  std::mutex deque_mtx;

  // http Post server
  device::interface::post_server::ServerConfig post_server_config;
  post_server_config.name = "POST_SERVER";
  post_server_config.bind_ip = common::config::GetEnvVariable(
      kENVBindIp, server_config["server_ip"].get<std::string>());
  post_server_config.bind_port = common::config::GetEnvVariable(
      kENVBindPort, server_config["server_port"].get<std::string>());
  post_server_config.bind_url = common::config::GetEnvVariable(
      kENVBindURL, server_config["server_url"].get<std::string>());

  device::interface::post_server::PostServerInterface post_server(
      post_server_config);
  post_server.RegisterCallback([&post_data_deque, &deque_mtx](
        const std::string data) {
    std::lock_guard<std::mutex> guard(deque_mtx);
    post_data_deque.push_front(data);
  });

  // Setup Redis
  std::string redis_ip = common::config::GetEnvVariable(kENVRedisIp, kRedisIp);
  int redis_port = stoi(common::config::GetEnvVariable(
      kENVRedisPort, kRedisPort));
  std::string flir_pub_topic = common::config::GetEnvVariable(
      kENVPubChannel, PUB_CHANNEL);
  redisContext *redis_context = redisConnect(redis_ip.c_str(), redis_port);
  if (redis_context->err) {
    printf("redis error: %s\n", redis_context->errstr);
    return 1;
  }

  // Start http post server.
  post_server.Start();
  // Process incoming data.
  while(true) {
    std::string payload;
    std::string payload_base64;
    std::string payload_json;
    std::this_thread::sleep_for(
      std::chrono::milliseconds(kDefaultWaitNextLoopTimeMS));
    if (!post_data_deque.empty()) {
      const std::lock_guard<std::mutex> guard(deque_mtx);
      payload = post_data_deque.front();
      post_data_deque.pop_front();
      while(post_data_deque.size() > kMaxDequeBufferSize) {
        post_data_deque.pop_back();
      }
    } else {
      continue;
    }
    // debase
    payload_base64 = base64_decode(payload);
    encryption_util.DecryptData(payload_base64, &payload_json);
    redisCommand(redis_context, "PUBLISH %s %s",
        flir_pub_topic.c_str(), payload_json.c_str());
    std::cout << "Published a new frame with length: "
        << strlen(payload_json.c_str())
        << ", sample data: " << payload_json.substr(0, 30) << std::endl;
  }
  return 0;
}

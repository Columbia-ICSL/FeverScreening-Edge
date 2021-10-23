//
// Copyright 2020 ICSL. All Rights Reserved.
//

#ifndef DEVICE_HTTP_POST_SERVER_INTERFACE_H_
#define DEVICE_HTTP_POST_SERVER_INTERFACE_H_

#include <chrono>
#include <functional>
#include <iostream>
#include <string>
#include <thread>

#include <dirent.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "event2/buffer.h"
#include "event2/event.h"
#include "event2/http.h"
#include "event2/keyvalq_struct.h"
#include "event2/listener.h"
#include "event2/util.h"

#include <glog/logging.h>

#ifdef EVENT__HAVE_NETINET_IN_H
#include <netinet/in.h>
#ifdef _XOPEN_SOURCE_EXTENDED
#include <arpa/inet.h>
#endif
#endif

namespace device {
namespace interface {
namespace post_server {
struct ServerConfig {
  std::string name = "default";
  std::string bind_ip = "127.0.0.1";
  std::string bind_port = "10021";
  std::string bind_url = "/fever-service";
  bool unlink = true;
};

class PostServerInterface {
 public:
  PostServerInterface(const ServerConfig& server_config);
  ~PostServerInterface();

  void RegisterCallback(std::function<void(const std::string)>);
  void SetServerConfig(const ServerConfig& server_config);
  void Start();
  void Stop();
  bool isRunning() const;

 private:
  ServerConfig server_config_;
  struct event_config* event_config_;
  struct event_base* event_base_;
  struct evhttp* event_http_;
  struct evhttp_bound_socket* event_handle_;
  struct event* event_term_;
  std::vector<uint8_t> char_buf_;
  std::thread runner_;

  bool is_running_;
  bool can_run_;
  bool callback_set_;

  std::function<void(const std::string)> callback_;

  static void HandleRequest(struct evhttp_request *req, void* arg);
  static void TerminateServerCallback(int sig, short events, void* arg);
  bool OpenSocket();
};  // class PostServerInterface
}  // namespac post_server
}  // namespace interface
}  // namespace device

#endif  // DEVICE_HTTP_POST_SERVER_INTERFACE_H_
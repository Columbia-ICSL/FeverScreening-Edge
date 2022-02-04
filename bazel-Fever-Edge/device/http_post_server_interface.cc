//
// Copyright 2020 ICSL. All Rights Reserved.
//

#include "device/http_post_server_interface.h"

namespace device {
namespace interface {
namespace post_server {
namespace {
constexpr int kDefaultWaitExitingTimeMS = 100;
constexpr int kDefaultNextLoopTimeMS = 10;
}

PostServerInterface::PostServerInterface(const ServerConfig& server_config) :
    event_config_(nullptr),
    event_base_(nullptr),
    event_http_(nullptr),
    event_handle_(nullptr),
    event_term_(nullptr),
    is_running_(false),
    can_run_(false),
    callback_set_(false) {
  SetServerConfig(server_config);
  return;
}
PostServerInterface::~PostServerInterface() {
  Stop();
  return;
}

void PostServerInterface::RegisterCallback(
    std::function<void(const std::string)> callback) {
  callback_ = callback;
  callback_set_ = true;
}

void PostServerInterface::SetServerConfig(const ServerConfig& server_config) {
  if (is_running_) {
    can_run_ = false;
    Stop();
  }
  server_config_ = server_config;
  can_run_ = true;
}

void PostServerInterface::Start() {
  can_run_ = true;
  if (!callback_set_) {
    LOG(INFO) << "Server is not ready to run";
    return;
  }
  event_config_ = event_config_new();
  event_base_ = event_base_new_with_config(event_config_);
  if (!event_base_) {
    LOG(ERROR) << "Couldn't create an event_base: exiting";
    return;
  }
  event_config_free(event_config_);
  event_config_ = nullptr;
  event_http_ = evhttp_new(event_base_);
  if (!event_http_) {
    LOG(ERROR) << "couldn't create evhttp. Exiting.";
    return;
  }

  /* The /dump URI will dump all requests to stdout and say 200 ok. */
  evhttp_set_cb(event_http_,
      server_config_.bind_url.c_str(), HandleRequest,
      reinterpret_cast<void*>(this));

  event_handle_ = evhttp_bind_socket_with_handle(
      event_http_, server_config_.bind_ip.c_str(),
      stoi(server_config_.bind_port));
  if (!event_handle_) {
    LOG(ERROR) << "Couldn't bind to " << server_config_.bind_ip
        << ":" << server_config_.bind_port << ". Exiting";
    goto err;
  }

  if (OpenSocket() == false) {
    LOG(ERROR) << "Cannot setup socket.";
    goto err;
  }

  event_term_ = evsignal_new(event_base_,
      SIGINT, TerminateServerCallback, event_base_);
  if (!event_term_) {
    goto err;
  }
  if (event_add(event_term_, NULL)) {
    goto err;
  }

  is_running_ = true;
  // event_base_dispatch(event_base_);
  runner_ = std::thread([this](){
    while(this->can_run_) {
      event_base_loop(this->event_base_, EVLOOP_ONCE);
      std::this_thread::sleep_for(
        std::chrono::milliseconds(kDefaultNextLoopTimeMS));
    }
  });
  runner_.detach();
  return;
  // event_base_dispatch(event_base_);

err:
  if (event_config_) event_config_free(event_config_);
  if (event_http_) evhttp_free(event_http_);
  if (event_term_) event_free(event_term_);
  if (event_base_) event_base_free(event_base_);
}

void PostServerInterface::Stop() {
  can_run_ = false;
  // Stop the server.
  while(is_running_) {
    std::this_thread::sleep_for(
        std::chrono::milliseconds(kDefaultWaitExitingTimeMS));
  }
  return;
}

bool PostServerInterface::isRunning() const {
  return is_running_;
}

void PostServerInterface::HandleRequest(
    struct evhttp_request* req, void* arg) {
  PostServerInterface* server = reinterpret_cast<PostServerInterface*>(arg);
  struct evbuffer *buf;
  size_t buf_size;
  std::string buf_str;

  if (evhttp_request_get_command(req) != EVHTTP_REQ_POST) {
    // Not the request we want
    LOG(WARNING) << "Received a non-POST request. Probably an attack";
  } else {
    buf = evhttp_request_get_input_buffer(req);
    buf_size = evbuffer_get_length(buf);
    if (server->char_buf_.size() < buf_size) {
      server->char_buf_.resize(2 * buf_size);
    }
    buf_str.reserve(buf_size);
    evbuffer_remove(buf, server->char_buf_.data(), buf_size);
    buf_str.assign(server->char_buf_.begin(),
        server->char_buf_.begin() + buf_size);
    server->callback_(std::move(buf_str));
  }

  evhttp_send_reply(req, 200, "OK", NULL);
}

void PostServerInterface::TerminateServerCallback(
    int sig, short events, void* arg) {
  PostServerInterface* server = reinterpret_cast<PostServerInterface*>(arg);
  event_base_loopbreak(server->event_base_);
  LOG(INFO) << "Terminating the server";
  server->is_running_ = false;
}

bool PostServerInterface::OpenSocket() {
  struct sockaddr_storage ss;
  evutil_socket_t fd;
  ev_socklen_t socklen = sizeof(ss);
  char addrbuf[128];
  void *inaddr;
  const char *addr;
  int got_port = -1;

  fd = evhttp_bound_socket_get_fd(event_handle_);
  memset(&ss, 0, sizeof(ss));
  if (getsockname(fd, (struct sockaddr *)&ss, &socklen)) {
    LOG(ERROR) << "getsockname() failed";
    return false;
  }

  if (ss.ss_family == AF_INET) {
    got_port = ntohs(((struct sockaddr_in *)&ss)->sin_port);
    inaddr = &((struct sockaddr_in *)&ss)->sin_addr;
  } else if (ss.ss_family == AF_INET6) {
    got_port = ntohs(((struct sockaddr_in6 *)&ss)->sin6_port);
    inaddr = &((struct sockaddr_in6 *)&ss)->sin6_addr;
  } else {
    LOG(ERROR) << "Weird address family "
        << static_cast<int>(ss.ss_family);
    return false;
  }

  addr = evutil_inet_ntop(ss.ss_family, inaddr, addrbuf, sizeof(addrbuf));
  if (addr) {
    printf("Listening on %s:%d\n", addr, got_port);
  } else {
    fprintf(stderr, "evutil_inet_ntop failed\n");
    return false;
  }

  return true;
}
}  // namespace post_server
}  // namespace interface
}  // namespace device

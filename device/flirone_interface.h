//
// Copyright 2020 ICSL. All Rights Reserved.
//

#ifndef DEVICE_FLIRONE_INTERFACE_H_
#define DEVICE_FLIRONE_INTERFACE_H_

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include <atomic>
#include <chrono>
#include <deque>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <glog/logging.h>

#include "include/libusb-1.0/libusb.h"

#include "common/status.h"
#include "common/time.h"
#include "device/flirone_datatypes.h"

namespace device {
namespace impl_ {
constexpr uint8_t kPingPongSize = 2;
}

struct FlirOneConfig {
  std::string name;
  std::string machine_id;
};

struct FlirOneDeviceConfig {
  uint8_t bmRequestType = 0x1;
  uint8_t bRequest = 0xb;
  uint8_t wValue = 0x1;
  uint8_t stop_start = 0x0;
  uint8_t wIndex = 0x1;
  uint8_t wLength = 0x0;
  uint8_t data[2] = {0x0, 0x0};
  uint8_t magic_byte[4] = {0xEF, 0xBE, 0x00, 0x00};
};

struct FlirOneDeviceModel {
  uint32_t ep85_buf_size = 0;
  uint32_t ep85_head_idx = 0;
  uint32_t sequence_count = 0;
  struct libusb_device_handle *dev_handle = NULL;
  FlirOneDeviceConfig config;
  flir::FlirOneCalibProfile calib_profile;
  char ep81_error[kEPErrorSize];
  char ep83_error[kEPErrorSize];
  char ep85_error[kEPErrorSize];
  uint8_t frame_buf[kBuf85Size];
  uint8_t file_buf[kBufFileSize];
  uint8_t file_buf_size = 0;
  uint8_t config_buf[kBufConfigSize];
  uint8_t null_buf[kBufFileSize];
};

class FlirOneInterface {
 public:
  explicit FlirOneInterface(const FlirOneConfig config);
  ~FlirOneInterface();

  bool isOpen() const;
  bool hasNewFrame() const;
  common::Status Run();
  void Stop();

  common::Status PollGetResult(FlirOneImageResult* result);

  // Not available for now
  // common::Status RegisterCallback(std::function);

 private:
  bool opened_;
  bool runnable_;
  int8_t pingpong_;
  bool has_new_frame_;
  std::mutex swap_cache_mtx_;
  FlirOneConfig config_;
  FlirOneImageResult result_cache_[impl_::kPingPongSize];
  FlirOneDeviceModel device_;
  FlirOneDeviceConfig device_config_;
  std::shared_ptr<std::thread> thread_run_;

  static void GetResultImpl(FlirOneInterface* interface);
  static void RequestNewFrame(FlirOneInterface* interface);

};
}  // namespace device
#endif  // DEVICE_FLIRONE_INTERFACE_H_
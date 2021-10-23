//
// Copyright 2020 ICSL. All Rights Reserved.
//

#include "device/flirone_interface.h"

namespace device {
namespace {
constexpr uint32_t kVendor_id = 0x09cb;
constexpr uint32_t kProduct_id = 0x1996;
constexpr uint32_t kMinSizeParsingPayload = 28;
}  // namespace

// Private helper functions
int FindLvrFlirUsb(FlirOneDeviceModel *device) {
  device->dev_handle =
      libusb_open_device_with_vid_pid(NULL, kVendor_id, kProduct_id);
  if (device->dev_handle == NULL || device->dev_handle == nullptr) {
    return -EIO;
  } else {
    return 0;
  }
}

int BulkTransferHelper(
    libusb_device_handle *dev_handle,
    unsigned char endpoint,
    unsigned char *data,
    int length,
    int *actual_length,
    std::string purpose,
    bool is_write) {
  int ret = libusb_bulk_transfer(dev_handle, endpoint, data,
      length, actual_length, 100);
  if (ret == 0
      && ((*actual_length == length && is_write) || (!is_write))) {
    printf("\n%s: sucessful for the data with length %d: \n",
        purpose.c_str(), *actual_length);
    printf("%.*s\n", *actual_length, data);
    for (int i = 0; i < *actual_length; i++) {
      printf("%02X ", data[i]);
    }
    printf("\n");
  } else {
    printf("%s: Error! res = %d and transferred = %d\n",
        purpose.c_str(), ret, *actual_length);
  }
  return ret;
}

int EP02AskCameraFiles(FlirOneDeviceModel *device) {
  //--------- write string:
  //{"type":"openFile","data":{"mode":"r","path":"CameraFiles.zip"}}
  const int ctrl_msg_len = 16;
  int msg_transferred = 0;
  int ret = 0;
  uint8_t ctrl_msg[16] = {0xcc, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
                          0x41, 0x00, 0x00, 0x00, 0xF8, 0xB3, 0xF7, 0x00};
  std::string payload_msg =
      "{\"type\":\"openFile\",\"data\":{\"mode\":\"r\",\"path\":"
      "\"CameraFiles.zip\"}}\n";
  unsigned char payload_char[payload_msg.length()];
  std::copy(payload_msg.begin(), payload_msg.end(), payload_char);

  ret = BulkTransferHelper(device->dev_handle, 2, ctrl_msg, ctrl_msg_len,
      &msg_transferred, "EP02AskCameraFiles: Writing ctrl_msg", true);
  ret = BulkTransferHelper(device->dev_handle, 2, payload_char,
      payload_msg.length(), &msg_transferred,
      "EP02AskCameraFiles: Writing payload_msg", true);

  ret = BulkTransferHelper(device->dev_handle, 0x83, device->file_buf,
      kBufFileSize, &msg_transferred,
      "EP02AskCameraFile, read camera file", false);
  if (ret == 0) {
    // Got CameraFiles.
    device->file_buf_size = msg_transferred;
  }
  return ret;
}

int EP02AskStreameId(FlirOneDeviceModel *device) {
  //--------- write string:
  //{"type":"openFile","data":{"mode":"r","path":"CameraFiles.zip"}}
  const int ctrl_msg_len = 16;
  int msg_transferred = 0;
  int ret = 0;
  uint8_t ctrl_msg[16] = {0xcc, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
                          0x33, 0x00, 0x00, 0x00, 0xef, 0xdb, 0xc1, 0xc1};
  std::string payload_msg =
      "{\"type\":\"readFile\",\"data\":{\"streamIdentifier\":10}}\n";
  unsigned char payload_char[payload_msg.length()];
  std::copy(payload_msg.begin(), payload_msg.end(), payload_char);

  ret = BulkTransferHelper(device->dev_handle, 2, ctrl_msg, ctrl_msg_len,
      &msg_transferred, "EP02AskStreameId: Writing  ctrl_msg", true);

  ret = BulkTransferHelper(device->dev_handle, 2, payload_char,
      payload_msg.length(), &msg_transferred,
      "EP02AskStreameId: Writing  payload_msg", true);

  ret = BulkTransferHelper(device->dev_handle, 0x83, device->file_buf,
      kBufFileSize, &msg_transferred, "EP02AskStreamId: get result", false);
  return ret;
}

int EP85AskVideo(FlirOneDeviceModel *device) {
  int ret = 0;
  printf("\nAsk for video stream, start EP 0x85:\n");
  ret = libusb_control_transfer(device->dev_handle, 1, 0x0b, 1, 2,
                                device->config.data, 2, 200);
  if (ret < 0) {
    std::cout << "Control Out error " << ret << std::endl;
  }
  // libusb_control_transfer returns bytes transferred.
  return ret < 0;
}

int FlirOneUsbInit(FlirOneDeviceModel *device) {
  int ret = 0;
  ret = libusb_init(NULL);
  if (ret < 0) {
    fprintf(stderr, "failed to initialise libusb\n");
    return ret;
  }
  ret = FindLvrFlirUsb(device);
  if (ret < 0) {
    fprintf(stderr, "Could not find/open device\n");
    goto out;
  }
  printf("Successfully find the Flir One G2 device\n");
  ret = libusb_set_configuration(device->dev_handle, 3);
  if (ret < 0) {
    fprintf(stderr, "libusb_set_configuration error %d\n", ret);
    goto out;
  }
  printf("Successfully set usb configuration 3\n");

  // Claiming of interfaces is a purely logical operation;
  // it does not cause any requests to be sent over the bus.
  ret = libusb_claim_interface(device->dev_handle, 0);
  if (ret < 0) {
    fprintf(stderr, "libusb_claim_interface 0 error %d\n", ret);
    goto out;
  }
  ret = libusb_claim_interface(device->dev_handle, 1);
  if (ret < 0) {
    fprintf(stderr, "libusb_claim_interface 1 error %d\n", ret);
    goto out;
  }
  ret = libusb_claim_interface(device->dev_handle, 2);
  if (ret < 0) {
    fprintf(stderr, "libusb_claim_interface 2 error %d\n", ret);
    goto out;
  }
  printf("Successfully claimed interface 0,1,2\n");
  return ret;
out:
  // close the device
  libusb_reset_device(device->dev_handle);
  libusb_close(device->dev_handle);
  libusb_exit(NULL);
  return ret;
}

int FlirOneUsbConfig(FlirOneDeviceModel *device) {
  // Flir config
  //   01 0b 01 00 01 00 00 00 c4 d5
  //   0 bmRequestType = 01
  //   1 bRequest = 0b
  //   2 wValue 0001 type (H) index (L)
  //   3 stop=0/start=1 (Alternate Setting)
  //   4 wIndex 01 interface 1/2
  //   5 wLength 00
  //   6 Data 00 00
  // libusb_control_transfer (*dev_handle, bmRequestType, bRequest,
  //    wValue,  wIndex, *data, wLength, timeout)

  int ret = 0;
  printf("stop interface 2 FRAME\n");
  ret = libusb_control_transfer(device->dev_handle, 1, 0x0b, 0, 2,
                                device->config.data, 0, 100);
  if (ret < 0) {
    fprintf(stderr, "Control Out error %d\n", ret);
    return ret;
  }

  printf("stop interface 1 FILEIO\n");
  ret = libusb_control_transfer(device->dev_handle, 1, 0x0b, 0, 1,
                                device->config.data, 0, 100);
  if (ret < 0) {
    fprintf(stderr, "Control Out error %d\n", ret);
    return ret;
  }

  printf("\nstart interface 1 FILEIO\n");
  ret = libusb_control_transfer(device->dev_handle, 1, 0x0b, 1, 1,
                                device->config.data, 0, 100);
  if (ret < 0) {
    fprintf(stderr, "Control Out error %d\n", ret);
    return ret;
  }

  // Suppose to read 
  int actual_length;
  ret = BulkTransferHelper(device->dev_handle, 0x81, device->file_buf,
      kBuf85Size, &actual_length,
      "Config, header, expect how many char to receive.", false);
  ret = BulkTransferHelper(device->dev_handle, 0x81, device->file_buf,
      kBuf85Size, &actual_length,
      "Config, should be a long serial information.", false);
  return ret;
}

int FindJpgTrailingBytes(const uint8_t *buf, int32_t trailing_addr) {
  int ip = 0;
  int trailing_bytes = 0;
  int found_d9 = 0;
  int found_ff = 0;
  uint8_t byte;
  for (ip = 0; ip < 10; ip++) {
    byte = buf[trailing_addr - ip];
    if (found_ff == 1) {
      continue;
    }
    if (byte == 0) {
      trailing_bytes += 1;
      continue;
    }
    if (byte == 255) {
      if (found_d9 == 1) {
        found_ff = 1;
        continue;
      } else {
        printf("ff but no d9!\n");
      }
    }
    if (byte == 217) {
      found_d9 = 1;
      continue;
    }
    trailing_bytes += 1;
  }
  return trailing_bytes;
}

FlirOneInterface::FlirOneInterface(const FlirOneConfig config)
    : opened_(false), config_(config) {
  thread_run_ = nullptr;
  runnable_ = false;
  for (auto& result : result_cache_) {
    result.machine_id = config.machine_id;
  }
  pingpong_ = 0;
  has_new_frame_ = false;
  return;
}

FlirOneInterface::~FlirOneInterface() {
  // Stop the camera.
  Stop();
}

bool FlirOneInterface::isOpen() const { return opened_; }

common::Status FlirOneInterface::Run() {
  runnable_ = true;
  thread_run_ =
      std::make_shared<std::thread>(&FlirOneInterface::GetResultImpl, this);
  thread_run_->detach();
  opened_ = true;
  return common::Status::OK;
}

void FlirOneInterface::Stop() {
  runnable_ = false;
  std::this_thread::sleep_for(std::chrono::seconds(1));
  if (device_.dev_handle != NULL) {
    LOG(INFO) << "Force to close the device";
    libusb_release_interface(device_.dev_handle, 0);
    libusb_reset_device(device_.dev_handle);
    libusb_close(device_.dev_handle);
    libusb_exit(NULL);
  }
}

common::Status FlirOneInterface::PollGetResult(FlirOneImageResult *result) {
  if (result == nullptr) {
    return common::Status::INPUT_POINTER_IS_NULL;
  }
  while (!has_new_frame_) {
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    if (!opened_) {
      return common::Status::DEVICE_CLOSED;
    }
  }
  {
    std::lock_guard<std::mutex> guard(swap_cache_mtx_);
    has_new_frame_ = false;
    *result = result_cache_[1 - pingpong_];
    LOG(INFO) << "sequence: " << result_cache_[1 - pingpong_].sequence
        << ", ts: " << result_cache_[1 - pingpong_].timestamp_ns;
    return common::Status::OK;
  }
  return common::Status::ERROR;
}

void FlirOneInterface::GetResultImpl(FlirOneInterface *interface) {
  // Init libusb device.
  int ret = 0;
  if (FlirOneUsbInit(&(interface->device_)) != 0) {
      std::cout << "FlirOneUsbInit Failed" << std::endl;
  }
  interface->opened_ = true;
  // libusb state machine.
  int data_transferred;
  // save last error status to avoid clutter the log

  int state = 1;
  while (interface->runnable_) {
    switch (state) {
      case 1:
        if (FlirOneUsbConfig(&(interface->device_)) != 0) {
          std::cout << "FlironeUsbConfig Failed" << std::endl;
          goto out;
        }
        state = 3;  // jump over wait stait 2. Not really using any data from
                    // CameraFiles.zip
        break;
      case 2:
        if (EP02AskCameraFiles(&(interface->device_)) != 0) {
          std::cout << "EP02AskCameraFiles Failed" << std::endl;
          goto out;
        }
        if (EP02AskStreameId(&(interface->device_)) != 0) {
          std::cout << "EP02AskStreameId Failed" << std::endl;
          goto out;
        }
        // std::this_thread::sleep_for(std::chrono::seconds(1));
        state = 3;
        break;
      case 3:
        if (EP85AskVideo(&(interface->device_)) != 0) {
          std::cout << "EP85AskVideo Failed" << std::endl;
          goto out;
        }
        state = 4;
        // std::this_thread::sleep_for(std::chrono::seconds(1));
        break;
      case 4:
        // endless loop
        RequestNewFrame(interface);
        break;
    }
    // poll Endpoints 0x81, 0x83
    ret = libusb_bulk_transfer(interface->device_.dev_handle, 0x81,
                               interface->device_.null_buf, kBufFileSize,
                               &data_transferred, 10);
    ret = libusb_bulk_transfer(interface->device_.dev_handle, 0x83,
                               interface->device_.null_buf, kBufFileSize,
                               &data_transferred, 10);
    if (ret != 0 &&
        strcmp(libusb_error_name(ret), "LIBUSB_ERROR_NO_DEVICE") == 0) {
      fprintf(stderr, "EP 0x83 LIBUSB_ERROR_NO_DEVICE -> reset USB\n");
      goto out;
    }
  }
  // never reached ;-)
  libusb_release_interface(interface->device_.dev_handle, 0);

out:
  // close the device
  libusb_reset_device(interface->device_.dev_handle);
  libusb_close(interface->device_.dev_handle);
  libusb_exit(NULL);
  interface->device_.dev_handle = NULL;
  interface->opened_ = false;
  std::cout << "FlirOne Interface has been safetly released." << std::endl;
  return;
}

void FlirOneInterface::RequestNewFrame(FlirOneInterface *interface) {
  // poll Frame Endpoints 0x85
  // don't change timeout=100ms !
  int data_transferred = 0;
  int ret = 0;
  uint8_t *frame_buf = interface->device_.frame_buf + 0;
  uint32_t *ep85_buf_size = &(interface->device_.ep85_buf_size);
  uint32_t *ep85_head_idx = &(interface->device_.ep85_head_idx);
  ret = libusb_bulk_transfer(interface->device_.dev_handle, 0x85,
                             frame_buf + *ep85_buf_size, kBuf85Size,
                             &data_transferred, 100);
  *ep85_buf_size += data_transferred;
  if (data_transferred <= 0) {
    std::cout << "data_transferred is <=0\n";
    return;
  }
  if (ret < 0) {
    if (strcmp(const_cast<const char *>(interface->device_.ep85_error),
               libusb_error_name(ret)) != 0) {
      strcpy(interface->device_.ep85_error, libusb_error_name(ret));
      LOG(ERROR) << "RequestNewFrame error: " << interface->device_.ep85_error;
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    LOG(ERROR) << "RequestNewFrame error: " << interface->device_.ep85_error;
    return;
  }
  // Make sure the head is always magic bytes.
  for (uint32_t i = 0; i < *ep85_buf_size - 3; i++) {
    *ep85_head_idx = i;
    if (memcmp(frame_buf + *ep85_head_idx,
        interface->device_config_.magic_byte, 4) == 0) {
      break;
    }
  }
  if (*ep85_head_idx > 0) {
    memcpy(frame_buf, frame_buf + *ep85_head_idx,
        *ep85_buf_size - *ep85_head_idx);
    *ep85_buf_size -= *ep85_head_idx;
    *ep85_head_idx = 0;
  }

  // Check if data is enough to let us parse a new frame.
  if (*ep85_buf_size < kMinSizeParsingPayload) {
    // wait for next chunk.
    return;
  }
  // Parse frame size.
  uint32_t frame_size = frame_buf[8] + (frame_buf[9] << 8) +
                        (frame_buf[10] << 16) + (frame_buf[11] << 24);
  uint32_t thermal_size = frame_buf[12] + (frame_buf[13] << 8) +
                          (frame_buf[14] << 16) + (frame_buf[15] << 24);
  uint32_t jpg_size = frame_buf[16] + (frame_buf[17] << 8) +
                      (frame_buf[18] << 16) + (frame_buf[19] << 24);
  uint32_t status_size = frame_buf[20] + (frame_buf[21] << 8) +
                         (frame_buf[22] << 16) + (frame_buf[23] << 24);
  
  if ((frame_size + 28) > *ep85_buf_size) {
    // wait for next chunk
    return;
  }
  // Parse a frame.
  // std::cout << "Parse a new frame, frame size: " << frame_size
  //         << ", thermal_size: " << thermal_size << ", jpg_size: " << jpg_size
  //         << ", status_size: " << status_size
  //         << ", data in the buffer: " << *ep85_buf_size << std::endl;
  // Swap the cache


  FlirOneImageResult& frame = interface->result_cache_[interface->pingpong_];
  frame.timestamp_ns = common::GetSystemTimeNs();
  frame.sequence = interface->device_.sequence_count;
  // Fill thermal raw values.
  for (uint32_t y = 0; y < kThermalHeight; ++y) {
    for (uint32_t x = 0; x < kThermalWidth; ++x) {
      if (x < kThermalWidth / 2)
        frame.thermal_data[y * 160 + x] =
            frame_buf[2 * (y * 164 + x) + 32] +
            256 * frame_buf[2 * (y * 164 + x) + 33];
      else
        frame.thermal_data[y * 160 + x] =
            frame_buf[2 * (y * 164 + x) + 32 + 4] +
            256 * frame_buf[2 * (y * 164 + x) + 33 + 4];
    }
  }

  std::thread convert_temp([&frame](){
    frame.ConvertRawToTemp();
    });

  // Parse jpg image.
  int trailing_bytes = FindJpgTrailingBytes(frame_buf,
      kMinSizeParsingPayload + thermal_size + jpg_size);
  memcpy(frame.rgb_jpg, frame_buf + thermal_size + kMinSizeParsingPayload,
         jpg_size - trailing_bytes + 1);
  frame.rgb_jpg_size = jpg_size - trailing_bytes + 1;

  // Parse status.
  memcpy(frame.status_data, kMinSizeParsingPayload + frame_buf
      + thermal_size + jpg_size, status_size);
  frame.status_size = status_size;

  // Delete current frame
  memcpy(frame_buf, frame_buf + frame_size, *ep85_buf_size - frame_size);
  *ep85_buf_size -= frame_size;

  convert_temp.join();
  // Parse finished.
  // std::cout << "Parsed a new frame, ts_ns: " << frame.timestamp_ns
  //         << ", sequence: " << frame.sequence
  //         << ", jpg size: " << frame.rgb_jpg_size << std::endl;
  {
    std::lock_guard<std::mutex> lock_guard(interface->swap_cache_mtx_);
    interface->device_.sequence_count++;
    interface->pingpong_ = 1 - interface->pingpong_;
    interface->has_new_frame_ = true;
  }
}


FlirOneImageResult& FlirOneImageResult::operator = (
    const FlirOneImageResult &result) {
  machine_id = result.machine_id;
  timestamp_ns = result.timestamp_ns;
  sequence = result.sequence;
  thermal_width = result.thermal_width;
  thermal_height = result.thermal_height;
  thermal_channels = result.thermal_channels;
  thermal_depth = result.thermal_depth;
  thermal_size = result.thermal_size;
  thermal_data_size = result.thermal_data_size;
  thermal_data_type = result.thermal_data_type;
  tempearture_data_type = result.tempearture_data_type;
  rgb_width = result.rgb_width;
  rgb_height = result.rgb_height;
  rgb_channels = result.rgb_channels;
  rgb_depth = result.rgb_depth;
  rgb_jpg_size = result.rgb_jpg_size;
  status_size = result.status_size;
  // Copy payload.
  memcpy(thermal_data, result.thermal_data, sizeof(thermal_data));
  memcpy(rgb_jpg, result.rgb_jpg, sizeof(rgb_jpg));
  memcpy(status_data, result.status_data, sizeof(status_data));
  memcpy(temperature_data, result.temperature_data, sizeof(temperature_data));
  return *this;
}
}  // namespace device
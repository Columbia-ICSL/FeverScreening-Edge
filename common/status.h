#ifndef COMMON_STATUS_H_
#define COMMON_STATUS_H_
namespace common {

enum class Status : uint32_t {
  OK = 0,
  ERROR = 1,
  INPUT_POINTER_IS_NULL = 2,
  // Device related status.
  DEVICE_NOT_FOUND = 1000,
  DEVICE_CANNOT_OPEN = 1001,
  DEVICE_NO_RESPONSE = 1002,
  DEVICE_CLOSED = 1003,
};
}
#endif  // COMMON_STATUS_H_
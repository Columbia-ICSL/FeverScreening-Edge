#include "common/time.h"

namespace common {

int64_t GetSystemTimeNs() {
  return std::chrono::duration_cast<std::chrono::nanoseconds>(
      std::chrono::system_clock::now().time_since_epoch()).count();
}

int64_t GetSystemTimeUs() {
  return GetSystemTimeNs() / 1000;
}

int64_t GetSystemTimeMs() {
  return GetSystemTimeUs() / 1000;
}

int64_t GetSystemTimeS() {
  return GetSystemTimeMs() / 1000;
}
}
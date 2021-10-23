#ifndef COMMON_TIME_H_
#define COMMON_TIME_H_

#include <chrono>

namespace common {

int64_t GetSystemTimeNs();
int64_t GetSystemTimeUs();
int64_t GetSystemTimeMs();
int64_t GetSystemTimeS();
}
#endif  // COMMON_TIME_H_
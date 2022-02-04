//
// Copyright 2020 ICSL. All Rights Reserved.
//

#ifndef COMMON_ENCRYPTION_UTILS_H_

#include <string>
#include <vector>

#include "cryptopp/modes.h"
#include "cryptopp/aes.h"
#include "cryptopp/filters.h"
#include "cryptopp/osrng.h"
#include "glog/logging.h"

namespace common {
namespace encryption {

enum class EncryptionAlgo : uint8_t {
  AES_CBC = 0,
  AES_ECB = 1,
  AES_CCM = 2,
  AES_GCM = 3,
  AES_EAX = 4,
  AES_CFB = 5
};

// TODO: make creditians private and use friend to contro access.
class EncryptionProfile {
 public:
  std::string name;
  EncryptionAlgo algorithm;
  size_t key_length;
  size_t block_size;
  std::vector<uint8_t> key;
  std::vector<uint8_t> iv;

  EncryptionProfile();
  void ReadKeyIv(
    const std::string key, const std::string iv);
  void GenerateNewToken();
};

class EncryptionUtils {
 public:
  void SetEncryptionProfile(const EncryptionProfile& profile);
  EncryptionProfile GetEncryptionProfile() const;

  void EncryptData(
      const std::string& input,
      std::string* output);
  void DecryptData(
      const std::string& input,
      std::string* output);
 private:
  EncryptionProfile profile_;

  void EncryptCFBImpl_(const std::string& input,
      std::string* output);
  void DecryptCFBImpl_(const std::string& input,
      std::string* output);
};
}  // namespace encryption
}  // namespace common
#define COMMON_ENCRYPTION_UTILS_H_
#endif  // COMMON_ENCRYPTION_UTILS_H_
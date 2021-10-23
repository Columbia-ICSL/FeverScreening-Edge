//
// Copyright 2020 ICSL. All Rights Reserved.
//

#include <vector>

#include "gtest/gtest.h"
#include "common/encryption_utils.h"

namespace common {
namespace encryption {

TEST(encryption_utils, constructor_1) {
  EncryptionUtils encrypt_utils;
  std::string input;
  std::string cipher;
  std::string recovered;
  input.resize(209308);
  for (int i = 0; i < 209308; i++) {
    input[i] = i % 255;
  }
  encrypt_utils.EncryptData(input, &cipher);
  EXPECT_EQ(input.length(), cipher.length());
  // Decrypt
  encrypt_utils.DecryptData(cipher, &recovered);
  EXPECT_EQ(input.length(), recovered.length());
  EXPECT_EQ(input.compare(recovered), 0);
}


TEST(encryption_utils, constructor_2) {
  common::encryption::EncryptionProfile profile;
  profile.name = "ICSL";
  profile.algorithm = common::encryption::EncryptionAlgo::AES_CFB;
  profile.ReadKeyIv("1550deb2825dd162", "d08c571d450df540");
  EncryptionUtils encrypt_utils;
  std::string input;
  std::string cipher;
  std::string recovered;
  encrypt_utils.SetEncryptionProfile(profile);
  input.resize(209308);
  for (int i = 0; i < 209308; i++) {
    input[i] = i % 255;
  }
  encrypt_utils.EncryptData(input, &cipher);
  EXPECT_EQ(input.length(), cipher.length());
  // Decrypt
  encrypt_utils.DecryptData(cipher, &recovered);
  EXPECT_EQ(input.length(), recovered.length());
  EXPECT_EQ(input.compare(recovered), 0);
}
}  // namespace encryption
}  // namespace common
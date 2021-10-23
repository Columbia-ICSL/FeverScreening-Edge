//
// Copyright 2020 ICSL. All Rights Reserved.
//

#include <iostream>
#include "common/encryption_utils.h"

namespace common {
namespace encryption {
EncryptionProfile::EncryptionProfile() {
  key_length = CryptoPP::AES::DEFAULT_KEYLENGTH;
  block_size = CryptoPP::AES::BLOCKSIZE;
  GenerateNewToken();
}

void EncryptionProfile::GenerateNewToken() {
  name = "random";
  algorithm = EncryptionAlgo::AES_CFB;
  CryptoPP::AutoSeededRandomPool random_gen;
  // Generate a random key
  key.resize(key_length);
  random_gen.GenerateBlock(key.data(), key.size());
  // Generate a random IV
  iv.resize(block_size);
  random_gen.GenerateBlock(iv.data(), iv.size());
}

void EncryptionProfile::ReadKeyIv(
    const std::string new_key, const std::string new_iv) {
  if (new_key.length() != key_length ||
      new_iv.length() != block_size) {
    LOG(ERROR) << "Trying to read a key/iv with length mismatch!";
    return;
  }
  key.assign(new_key.begin(), new_key.end());
  iv.assign(new_iv.begin(), new_iv.end());
}

void EncryptionUtils::SetEncryptionProfile(const EncryptionProfile& profile) {
  profile_ = profile;
}

EncryptionProfile EncryptionUtils::GetEncryptionProfile() const {
  return profile_;
}

void EncryptionUtils::EncryptData(
    const std::string& input,
    std::string* output) {
  if (output == nullptr) {
    LOG(ERROR) << "Output pointer is nullptr";
    return;
  }
  if (profile_.name.length() <= 0) {
    LOG(ERROR) << "profile has not been setup!";
    return;
  }
  switch (profile_.algorithm) {
    case EncryptionAlgo::AES_CFB:
      EncryptCFBImpl_(input, output);
      break;
    default:
      LOG(WARNING) << "The encryption algorithm "
          << static_cast<int>(profile_.algorithm) << " is not supported yet";
  }
}

void EncryptionUtils::DecryptData(
    const std::string& input,
    std::string* output) {
  if (output == nullptr) {
    LOG(ERROR) << "Output pointer is nullptr";
    printf("Output pointer is nullptr\n");
    return;
  }
  if (profile_.name.length() <= 0) {
    LOG(ERROR) << "profile has not been setup!";
    return;
  }
  switch (profile_.algorithm) {
    case EncryptionAlgo::AES_CFB:
      DecryptCFBImpl_(input, output);
      break;
    default:
      LOG(WARNING) << "The decryption algorithm "
          << static_cast<int>(profile_.algorithm) << " is not supported yet";
  }
}

void EncryptionUtils::EncryptCFBImpl_(
    const std::string& input,
    std::string* output) {
  std::string cipher;
  CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption encryptor;
  encryptor.SetKeyWithIV(
      profile_.key.data(), profile_.key_length, profile_.iv.data());
  CryptoPP::StringSource(
    input, true, 
    new CryptoPP::StreamTransformationFilter(
    encryptor, new CryptoPP::StringSink(cipher),
    CryptoPP::BlockPaddingSchemeDef::NO_PADDING));
  *output = std::move(cipher);
}

void EncryptionUtils::DecryptCFBImpl_(
    const std::string& input,
    std::string* output) {
  CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption decryptor;
  decryptor.SetKeyWithIV(
      profile_.key.data(), profile_.key_length, profile_.iv.data());
  CryptoPP::StringSource(
    input, true, 
    new CryptoPP::StreamTransformationFilter(
    decryptor, new CryptoPP::StringSink(*output)));
}
}  // namespace encryption
}  // namespace COMMON
//================================= Includes ===================================
#include "utils/cipher/chacha20.h"
#ifdef ANDROID
#include <android/log.h>
#include <sstream>
#include <string>
#include <iomanip>
#define LOG_TAG "RainTextCore-ChaCha20"
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__))
#endif

#include <cryptopp/chacha.h>
#include <cryptopp/hex.h>
#include <iostream>

#include "utils/rain_text_core_utils.h"

//================================= Namespace ==================================
namespace rain_text_core {

//======================== Define helpful variables ============================

//======================= Define helpful structures ============================

//======================= Define helpful functions =============================

//======================= Aes public implementation ============================
ChaCha20::ChaCha20(const uint8_t cypher_index, const std::vector<uint8_t> &key,
                   const std::vector<uint8_t> &text)
    : ICipher(cypher_index, key, text),
      cypher_index_(cypher_index),
      key_(key),
      text_(text) {
  if (key.size() < 96) {
    auto err = std::string(
        "The key must be a vector greater than or equal to 128.\nYour vector "
        "has only " +
        std::to_string(key.size()) + " elements");
    throw std::invalid_argument(err.data());
  }
  if (text.empty()) {
    throw std::invalid_argument("The text vector must not be empty");
  }
  rain_text_core_utils::SplitKey(32, key_, splited_keys_);
  key_index_ = 0;
  init_vector_index_ = 0;
  pre_salt_index_ = 0;
}

void ChaCha20::Encrypt(std::vector<uint8_t> &output) {
  key_index_ = 0;
  init_vector_index_ = 0;
  pre_salt_index_ = 0;
  rain_text_core_utils::GetIV(splited_keys_, key_index_, init_vector_index_,
                              pre_salt_index_, init_vector_, 8);
  output = std::vector<uint8_t>(text_.size());

#ifdef ANDROID
  LOGI("Vector in ChaCha key: %s",
       rain_text_core_utils::vectorToString(splited_keys_[key_index_]).data());
  LOGI("Vector in ChaCha20 IV: %s",
       rain_text_core_utils::vectorToString(
           std::vector<uint8_t>(init_vector_,
                                init_vector_ + 8))
           .data());
#endif

  CryptoPP::ChaCha::Encryption enc;
  enc.SetKeyWithIV((const CryptoPP::byte *)splited_keys_[key_index_].data(), 32,
                   (CryptoPP::byte *)init_vector_, CryptoPP::ChaCha::IV_LENGTH);

  CryptoPP::StreamTransformationFilter stf(
      enc, new CryptoPP::ArraySink(&output[0], output.size()));
  stf.Put(text_.data(), text_.size());
  stf.MessageEnd();

#ifdef ANDROID
  LOGI("Vector in ChaCha20 after encrypt: %s",
       rain_text_core_utils::vectorToString(text_).data());
  LOGI("Vector size: %d", text_.size());
#endif

  output.push_back(pre_salt_index_);
  output.push_back(init_vector_index_);
  output.push_back(key_index_);
  output.push_back(cypher_index_);
}

void ChaCha20::Decrypt(std::vector<uint8_t> &output) {
  if (text_.back() != (uint8_t)cypher_index_) {
    throw std::length_error("Compatibility problem");
  }
  pre_salt_index_ = text_[(text_.size() - 4)];
  init_vector_index_ = text_[(text_.size() - 3)];
  key_index_ = text_[(text_.size() - 2)];

  for (int i = 0; i < 4; ++i) {
    text_.pop_back();
  }
#ifdef ANDROID
  LOGI("Vector in ChaCha20: %s",rain_text_core_utils::vectorToString(text_).data());
  LOGI("Vector size: %d",text_.size());
#endif

  rain_text_core_utils::GetIV(splited_keys_, key_index_, init_vector_index_,
                              pre_salt_index_, init_vector_, 8, true);

#ifdef ANDROID
  LOGI("Vector in ChaCha key: %s",
       rain_text_core_utils::vectorToString(splited_keys_[key_index_]).data());
  LOGI("Vector in ChaCha20 IV: %s",
       rain_text_core_utils::vectorToString(
           std::vector<uint8_t>(init_vector_,
                                init_vector_ + 8))
           .data());
#endif

  output = std::vector<uint8_t>(text_.size());

  CryptoPP::ChaCha::Decryption dec;
  dec.SetKeyWithIV((const CryptoPP::byte *)splited_keys_[key_index_].data(), 32,
                   (CryptoPP::byte *)init_vector_, CryptoPP::ChaCha::IV_LENGTH);

  CryptoPP::StreamTransformationFilter stf(
      dec, new CryptoPP::ArraySink(&output[0], output.size()));
  stf.Put(text_.data(), text_.size());
  stf.MessageEnd();
}

const std::vector<uint8_t> &ChaCha20::GetKey() const { return key_; }

void ChaCha20::SetKey(const std::vector<uint8_t> &key) {
  if (key.size() < 96) {
    auto err = std::string(
        "The key must be a vector greater than or equal to 128.\nYour vector "
        "has only " +
        std::to_string(key.size()) + " elements");
    throw std::invalid_argument(err.data());
  }
  key_ = key;
}

const std::vector<uint8_t> &ChaCha20::GetText() const { return text_; }

void ChaCha20::SetText(const std::vector<uint8_t> &text) {
  if (text.empty()) {
    throw std::invalid_argument("The text vector must not be empty");
  }
  text_ = text;
}
//======================= Aes private implementation ===========================

//=================== Aes tests functions implementation =======================
#ifdef ENABLE_TESTS

#endif
//===================== Implement helpful functions ============================

}  // namespace rain_text_core

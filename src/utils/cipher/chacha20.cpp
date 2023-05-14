//================================= Includes ===================================
#include "utils/cipher/chacha20.h"

#include <argon2.h>
#include <crypto++/chacha.h>
#include <cryptopp/hex.h>
#include <cryptopp/modes.h>
#include <cryptopp/osrng.h>
#include <cryptopp/sha3.h>

#include <iostream>
#include <iomanip>

#include "utils/rain_text_core_utils.h"

//================================= Namespace ==================================
namespace rain_text_core {

//======================== Define helpful variables ============================

//======================= Define helpful structures ============================

//======================= Define helpful functions =============================

//======================= Aes public implementation ============================
ChaCha20::ChaCha20(const uint8_t cypher_index, const std::vector<uint8_t> &key,
                   const std::vector<uint8_t> &text)
    : ICipher(cypher_index, key, text), cypher_index_(cypher_index), key_(key), text_(text) {
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
  key_index_ = nullptr;
  init_vector_index_ = nullptr;
  pre_salt_index_ = nullptr;
}

ChaCha20::~ChaCha20() {
  delete key_index_;
  delete init_vector_index_;
  delete pre_salt_index_;
}

void ChaCha20::Encrypt(std::vector<uint8_t> &output) {
  CreateInitVector();
  output = std::vector<uint8_t>(text_.size());
  CryptoPP::ChaCha::Encryption enc;
  enc.SetKeyWithIV((const CryptoPP::byte *)splited_keys_[*key_index_].data(),
                   32, (CryptoPP::byte *)init_vector_,
                   CryptoPP::ChaCha::IV_LENGTH);

  CryptoPP::StreamTransformationFilter stf(
      enc, new CryptoPP::ArraySink(&output[0], output.size()));
  stf.Put(text_.data(), text_.size());
  stf.MessageEnd();

  output.push_back(*pre_salt_index_);
  output.push_back(*init_vector_index_);
  output.push_back(*key_index_);
  output.push_back(cypher_index_);
}

void ChaCha20::Decrypt(std::vector<uint8_t> &output) {
  if (text_.back() != (uint8_t)cypher_index_) {
    throw std::length_error("Compatibility problem");
  }
  pre_salt_index_ = new uint8_t(text_[(text_.size() - 4)]);
  init_vector_index_ = new uint8_t(text_[(text_.size() - 3)]);
  key_index_ = new uint8_t(text_[(text_.size() - 2)]);

  for (int i = 0; i < 4; ++i) {
    text_.pop_back();
  }

  ComputeInitVector();
  output = std::vector<uint8_t>(text_.size());

  CryptoPP::ChaCha::Decryption dec;
  dec.SetKeyWithIV((const CryptoPP::byte *)splited_keys_[*key_index_].data(),
                   32, (CryptoPP::byte *)init_vector_, CryptoPP::ChaCha::IV_LENGTH);

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
void ChaCha20::CreateInitVector() {
  std::uniform_int_distribution<int> dist(0, splited_keys_.size() - 1);
  key_index_ = new uint8_t(dist(random_));
  init_vector_index_ = new uint8_t(dist(random_));
  while (key_index_ == init_vector_index_) {
    *init_vector_index_ = dist(random_);
  }
  pre_salt_index_ = new uint8_t(dist(random_));
  while (pre_salt_index_ == key_index_ ||
         pre_salt_index_ == init_vector_index_) {
    *pre_salt_index_ = dist(random_);
  }

  ComputeInitVector();
}

void ChaCha20::ComputeInitVector() {
  if (!key_index_ || !init_vector_index_ || !pre_salt_index_) {
    throw std::runtime_error(
        "key_index_ or init_vector_index_ or pre_salt_index_ missing");
  }
  auto pre_init_vector = splited_keys_[*init_vector_index_];
  auto pre_salt = splited_keys_[*pre_salt_index_];
  CryptoPP::SHA3_256 salt_hash;
  salt_hash.Update((CryptoPP::byte *)pre_salt.data(), pre_salt.size());
  std::string salt_text;
  salt_text.resize(salt_hash.DigestSize());
  salt_hash.Final((CryptoPP::byte *)&salt_text[0]);

  auto salt = std::vector<uint8_t>(salt_text.begin(), salt_text.end());

  argon2id_hash_raw(10, 1 << 10, 4, pre_init_vector.data(),
                    pre_init_vector.size(), salt.data(), salt.size(),
                    init_vector_, 8);
}

//=================== Aes tests functions implementation =======================
#ifdef ENABLE_TESTS

#endif
//===================== Implement helpful functions ============================

}  // namespace rain_text_core

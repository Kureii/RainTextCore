//================================= Includes ===================================
#include "utils/rain_text_core_utils.h"

#include <Argon2.h>
#include <cryptopp/sha3.h>

#include <random>

//================================= Namespace ==================================
namespace rain_text_core::rain_text_core_utils {
//======================== Define helpful variables ============================
std::random_device random;
//======================= Define helpful structures ============================

//======================= Define helpful functions =============================

//============== rain_text_core_utils public implementation ====================
void SplitKey(uint16_t byte_size, std::vector<uint8_t> &key,
              std::vector<std::vector<uint8_t>> &output) {
  auto output_size = key.size() / byte_size;

  for (int i = 0; i < key.size(); i += byte_size) {
    if (i < (byte_size * output_size)) {
      output.emplace_back(key.begin() + i, key.begin() + i + byte_size);
    }
  }
}

void GetIV(std::vector<std::vector<uint8_t>> &splited_keys, uint8_t &key_index,
           uint8_t &init_vector_index, uint8_t &pre_salt_index, uint8_t iv[],
           uint8_t iv_size, bool decrypt) {
  if (!decrypt) {
    std::uniform_int_distribution<int> dist(0, splited_keys.size() - 1);
    key_index = uint8_t(dist(random));
    init_vector_index = uint8_t(dist(random));
    while (key_index == init_vector_index) {
      init_vector_index = dist(random);
    }
    pre_salt_index = uint8_t(dist(random));
    while (pre_salt_index == key_index || pre_salt_index == init_vector_index) {
      pre_salt_index = dist(random);
    }
  }
  auto pre_init_vector = (splited_keys)[init_vector_index];
  auto pre_salt = (splited_keys)[pre_salt_index];

  CryptoPP::SHA3_256 salt_hash;
  salt_hash.Update((CryptoPP::byte *)pre_salt.data(), pre_salt.size());
  std::string salt_text;
  salt_text.resize(salt_hash.DigestSize());
  salt_hash.Final((CryptoPP::byte *)&salt_text[0]);

  auto salt = std::vector<uint8_t>(salt_text.begin(), salt_text.end());

  std::vector<unsigned char> temp =
      Argon2::Argon2id(pre_init_vector, salt, 10, 1 << 10, 4, iv_size);
  std::copy(temp.begin(), temp.end(), iv);
}

//============ rain_text_core_utils tests functions implementation =============
#ifdef ENABLE_TESTS

#endif

//===================== Implement helpful functions ============================

//=============================== Namespace end ================================

}  // namespace rain_text_core::rain_text_core_utils
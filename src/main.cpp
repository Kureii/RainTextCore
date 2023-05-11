#include <cryptopp/chacha.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/files.h>
#include <cryptopp/hex.h>
#include <cryptopp/modes.h>
#include <cryptopp/osrng.h>
#include <cryptopp/rijndael.h>
#include <cryptopp/sha3.h>
#include <argon2.h>

#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <chrono>

int main(int argc, char* argv[]) {
  std::random_device random;
  //---------------------------- prepare key -----------------------------------
  auto key = std::make_unique<std::vector<uint8_t>>(256);
  for (int i = 0; i < 256; ++i) {
    std::uniform_int_distribution<int> dist(0, 0xFF);
    (*key)[i] =dist(random);
  }
  //---------------------------- separate keys ---------------------------------
  std::vector<std::vector<uint8_t>> keys;
  for (int i = 0; i < key->size(); i += 32) {
    keys.emplace_back(key->begin() + i, key->begin() + i + 32);
  }
  key.reset();
  //---------------------------- print keys ------------------------------------
/*
  for(int i = 0; i < keys.size(); ++i) {
    std::cout << "vector[" << i + 1 << "]\t{";
    for (int j = 0; j < keys[i].size(); j++) {
      if (j != keys[i].size() - 1) {
        std::cout << (int)keys[i][j] << ", ";
      } else {
        std::cout << (int)keys[i][j] << "}" << std::endl;
      }
    }
  }
*/
  //---------------------------- select key ------------------------------------
  std::uniform_int_distribution<int> dist(0, keys.size() - 1);
  auto key_index = dist(random);
  auto aes_key = keys[key_index];
  //--------------------- generate Initialization Vector  ----------------------
  uint8_t init_vector[16];
  {
    auto init_vector_index = dist(random);
    while (key_index == init_vector_index) {
      init_vector_index = dist(random);
    }
    auto pre_init_vector = keys[init_vector_index];
    auto pre_salt_index = dist(random);
    while (pre_salt_index == key_index || pre_salt_index == init_vector_index) {
      pre_salt_index = dist(random);
    }
    auto pre_salt = keys[pre_salt_index];
    CryptoPP::SHA3_256 salt_hash;
    salt_hash.Update((CryptoPP::byte *)pre_salt.data(), pre_salt.size());
    std::string salt_text;
    salt_text.resize(salt_hash.DigestSize());
    salt_hash.Final((CryptoPP::byte *)&salt_text[0]);
    auto salt = std::vector<uint8_t>(salt_text.begin(), salt_text.end());

    argon2id_hash_raw(10, 1 << 10, 4, pre_init_vector.data(),
                      pre_init_vector.size(), salt.data(), salt.size(),
                      init_vector, 16);
  }
  //---------------------------- plain data ------------------------------------
  std::string plain_text = "Ahoj svete";
  std::vector<uint8_t> plain_text_uint8(plain_text.begin(), plain_text.end());

  std::vector<uint8_t> cipher_text;

  CryptoPP::AES::Encryption aes_encryption((const CryptoPP::byte *)aes_key.data(), CryptoPP::AES::MAX_KEYLENGTH); // Použijte MAX_KEYLENGTH pro AES-256
  CryptoPP::CBC_Mode_ExternalCipher::Encryption cbc_encryption(aes_encryption, (const CryptoPP::byte*)init_vector);

  CryptoPP::StreamTransformationFilter stf_encryptor(cbc_encryption, new CryptoPP::VectorSink(cipher_text));
  stf_encryptor.Put(plain_text_uint8.data(), plain_text_uint8.size());
  stf_encryptor.MessageEnd();/**/

  for (auto & i : cipher_text) {
    std::cout << std::hex << (int)i;
  }

  return 0;
}



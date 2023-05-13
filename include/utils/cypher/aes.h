 #ifndef RAINTEXTCORE_AES_H
#define RAINTEXTCORE_AES_H

#include <iostream>
#include <vector>
#include <random>

namespace rain_text_core {

class Aes {
  const uint8_t cypher_index_;
  uint8_t * key_index_;
  uint8_t * init_vector_index_;
  uint8_t * pre_salt_index_;
  uint8_t init_vector_[16];
  std::random_device random_;
  std::vector<uint8_t> key_;
  std::vector<uint8_t> text_;
  std::vector<std::vector<uint8_t>> splited_keys_;

 public:
  explicit Aes(uint8_t cypher_index,
      const std::vector<uint8_t>& key,
               const std::vector<uint8_t>& text);
  void Encrypt(std::vector<uint8_t>& output);
  void Decrypt(std::vector<uint8_t>& output);
 private:
  void CreateInitVector();
  void ComputeInitVector();

};

}

#endif  // RAINTEXTCORE_AES_H

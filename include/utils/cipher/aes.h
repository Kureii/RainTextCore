#ifndef RAINTEXTCORE_AES_H
#define RAINTEXTCORE_AES_H

#include <iostream>
#include <random>
#include <vector>

namespace rain_text_core {

class Aes {
  const uint8_t cypher_index_;
  uint8_t* key_index_;
  uint8_t* init_vector_index_;
  uint8_t* pre_salt_index_;
  uint8_t init_vector_[16];
  std::random_device random_;
  std::vector<uint8_t> key_;
  std::vector<uint8_t> text_;
  std::vector<std::vector<uint8_t>> splited_keys_;

 public:
  explicit Aes(uint8_t cypher_index, const std::vector<uint8_t>& key,
               const std::vector<uint8_t>& text);
  virtual ~Aes();
  void Encrypt(std::vector<uint8_t>& output);
  void Decrypt(std::vector<uint8_t>& output);
  [[nodiscard]] const std::vector<uint8_t>& GetKey() const;
  void SetKey(const std::vector<uint8_t>& key);
  [[nodiscard]] const std::vector<uint8_t>& GetText() const;
  void SetText(const std::vector<uint8_t>& text);

 private:
  void CreateInitVector();
  void ComputeInitVector();
};

}  // namespace rain_text_core

#endif  // RAINTEXTCORE_AES_H

#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "core/rain_text_core.h"
#include "utils/cipher/aes.h"

void PrintVector(const std::vector<uint8_t>& vec) {
  std::cout << "{";
  for (auto & number:vec) {
    std::cout << "0x" << std::setw(2) << std::setfill('0') << std::hex << (int)number << ", ";
  }
  std::cout << "\b\b}"<<std::endl;
}

int main(int argc, char* argv[]) {
  std::random_device random;
  //---------------------------- prepare key -----------------------------------
  auto key = std::vector<uint8_t>(256);
  for (int i = 0; i < 256; ++i) {
    std::uniform_int_distribution<int> dist(0, 0xFF);
    key[i] =dist(random);
  }
  //---------------------------- plain data ------------------------------------
  std::string plain_text = "Ahoj svete";
  std::vector<uint8_t> plain_text_uint8(plain_text.begin(), plain_text.end());

  //auto rtc = rain_text_core::RainTextCore(*key, plain_text_uint8);

  auto aes = new rain_text_core::Aes(0, key, plain_text_uint8);

  std::vector<uint8_t> crypted_text;
  aes->Encrypt(crypted_text);

  std::cout << "Crypted text: ";
  PrintVector(crypted_text);
  std::vector<uint8_t> plain_text2;

  delete aes;
  auto aes_d = new rain_text_core::Aes(0, key, crypted_text);

  aes_d->Decrypt(plain_text2);
  std::cout << "Decrypted text: ";
  PrintVector(plain_text2);
  auto plain_text2_str = std::string(plain_text2.begin(), plain_text2.end());
  std::cout << "Text:" << plain_text2_str << std::endl;

  return 0;
}




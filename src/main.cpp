#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "core/rain_text_core.h"
#include "utils/cipher/chacha20.h"

void PrintVector(const std::vector<uint8_t>& vec) {
  std::cout << "{";
  for (auto& number : vec) {
    std::cout << "0x" << std::setw(2) << std::setfill('0') << std::hex
              << (int)number << ", ";
  }
  std::cout << "\b\b}" << std::endl;
}

int main(int argc, char* argv[]) {
  std::random_device random;
  //---------------------------- prepare key -----------------------------------
  auto key = std::vector<uint8_t>(256);
  for (int i = 0; i < 256; ++i) {
    std::uniform_int_distribution<int> dist(0, 0xFF);
    key[i] = dist(random);
  }
  //---------------------------- plain data ------------------------------------
  std::string plain_text = "Fuck microsoft";
  std::cout << "Plain text: " << plain_text << std::endl;
  std::vector<uint8_t> plain_text_uint8(plain_text.begin(), plain_text.end());
  // std::cout << "Plain text: ";  PrintVector(plain_text_uint8);std::cout <<
  // std::endl;

  auto rtc = new rain_text_core::RainTextCore(12, key, plain_text_uint8);

  std::vector<uint8_t> cipher;
  rtc->Encrypt(cipher);

  // std::cout << "cipher text: ";
  // PrintVector(cipher);
  rtc->SetText(cipher);
  std::vector<uint8_t> decrypted_text;

  rtc->Decrypt(decrypted_text);
  // std::cout << "Decrypted text: ";
  // PrintVector(decrypted_text);
  auto plain_text2_str =
      std::string(decrypted_text.begin(), decrypted_text.end());
  std::cout << "Decrypted text: " << plain_text2_str << std::endl;
  delete rtc;

  return 0;
}

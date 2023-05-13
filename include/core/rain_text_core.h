#ifndef RAINTEXTCORE_RAIN_TEXT_CORE_H
#define RAINTEXTCORE_RAIN_TEXT_CORE_H

#include <vector>
#include <iostream>
#include <random>
namespace rain_text_core {


class RainTextCore{
  std::vector<uint8_t> key_;
  std::vector<uint8_t> text_;
  std::random_device random_;
 public:
  RainTextCore(const std::vector<uint8_t> &key,
               const std::vector<uint8_t> &text);

 private:
};

}


#endif  // RAINTEXTCORE_RAIN_TEXT_CORE_H

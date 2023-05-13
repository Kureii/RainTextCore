//================================= Includes ===================================
#include "core/rain_text_core.h"
#include "utils/rain_text_core_utils.h"

#include <exception>

#include <cryptopp/cryptlib.h>
#include <cryptopp/hex.h>
#include <cryptopp/modes.h>
#include <cryptopp/osrng.h>
#include <cryptopp/rijndael.h>
#include <cryptopp/sha3.h>
#include <argon2.h>

#include <iomanip>

//================================= Namespace ==================================
namespace rain_text_core {
//======================== Define helpful variables ============================

//======================= Define helpful structures ============================

//======================= Define helpful functions =============================

//================== RainTextCore public implementation ========================
RainTextCore::RainTextCore(const std::vector<uint8_t>& key,
                           const std::vector<uint8_t>& text)
    : key_(key), text_(text) {
  std::vector<std::vector<uint8_t>> tmp;
  rain_text_core_utils::SplitKey(32, key_,  tmp);
  for(int i = 0; i < tmp.size(); ++i) {
    std::cout << "key[" << (int)i << "]:\t{";
    for(auto & j : tmp[i]) {
      std::cout << std::setw(2) << std::setfill('0') << std::hex << (int)j << ", ";
    }
    std::cout << "\b\b}\tsize:\t" << std::dec << (int)tmp[i].size() <<std::endl;
  }
}

//================== RainTextCore private implementation =======================


//============== database_utils tests functions implementation =================
#ifdef ENABLE_TESTS

#endif

//===================== Implement helpful functions ============================

//=============================== Namespace end ================================
}  // namespace rain_text_core

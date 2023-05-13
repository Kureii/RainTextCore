//================================= Includes ===================================
#include "utils/rain_text_core_utils.h"

//================================= Namespace ==================================
namespace rain_text_core::rain_text_core_utils {
//======================== Define helpful variables ============================

//======================= Define helpful structures ============================

//======================= Define helpful functions =============================

//============== rain_text_core_utils public implementation ====================
void SplitKey(uint16_t byte_size,
                                    std::vector<uint8_t>& key,
                                    std::vector<std::vector<uint8_t>>& output) {
  auto output_size = key.size() / byte_size;

  for (int i = 0; i < key.size(); i += byte_size) {
    if (i < (byte_size * output_size)) {
      output.emplace_back(key.begin() + i, key.begin() + i + byte_size);
    }
  }
}

//============ rain_text_core_utils tests functions implementation =============
#ifdef ENABLE_TESTS

#endif

//===================== Implement helpful functions ============================

//=============================== Namespace end ================================

}  // namespace rain_text_core::rain_text_core_utils
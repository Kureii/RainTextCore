//================================= Includes ===================================
#include "core/rain_text_core.h"

#include <exception>
#include <iomanip>
#include <vector>

#include "utils/cipher/aes.h"
#include "utils/cipher/chacha20.h"
#include "utils/cipher/twofish.h"
#include "utils/rain_text_core_utils.h"

//================================= Namespace ==================================
namespace rain_text_core {
//======================== Define helpful variables ============================

//======================= Define helpful structures ============================

//======================= Define helpful functions =============================

//================== RainTextCore public implementation ========================
RainTextCore::RainTextCore(uint16_t iterations, const std::vector<uint8_t>& key,
                           const std::vector<uint8_t>& text)
    : iterations_(iterations), key_(key), text_(text) {
  if (iterations == 0) {
    throw std::invalid_argument(
        "You must enter at least one iteration, we recommend at least 10");
  }
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
}
void RainTextCore::Encrypt(std::vector<uint8_t>& output) {

  text_.push_back(0xFF);

  std::uniform_int_distribution<> dist(0, NUM_CIPHERS - 1);
  std::vector<uint8_t> tmp;
  for (uint16_t i = 0; i < iterations_; ++i) {

    tmp = std::vector<uint8_t>();
    auto random_cipher_type = static_cast<CipherType>(dist(random_));
    std::unique_ptr<rain_text_core::ICipher> selected_cipher =
        CreateCipher(random_cipher_type, text_);
    selected_cipher->Encrypt(tmp);
    text_ = tmp;

  }
  output = tmp;
  text_ = std::move(tmp);
}
void RainTextCore::Decrypt(std::vector<uint8_t>& output) {

  std::vector<uint8_t> tmp;
  while (text_.back() != 0xFF) {
    tmp = std::vector<uint8_t>();
    auto cipher_index = static_cast<CipherType>(text_.back());

    std::unique_ptr<rain_text_core::ICipher> selected_cipher =
        CreateCipher(cipher_index, text_);

    selected_cipher->Decrypt(tmp);
    text_ = tmp;

  }
  tmp.pop_back();
  text_ = tmp;
  output = text_;
}

//================== RainTextCore private implementation =======================
std::unique_ptr<rain_text_core::ICipher> RainTextCore::CreateCipher(
    CipherType type, const std::vector<uint8_t>& text) {
  switch (type) {
    case AES:
      return std::make_unique<rain_text_core::Aes>(0, key_, text);
    case TWOFISH:
      return std::make_unique<rain_text_core::Twofish>(1, key_, text);
    case CHACHA20:
      return std::make_unique<rain_text_core::ChaCha20>(2, key_, text);
    default:
      throw std::invalid_argument("Invalid cipher type");
  }
}
const std::vector<uint8_t>& RainTextCore::GetText() const { return text_; }
void RainTextCore::SetText(const std::vector<uint8_t>& text) { text_ = text; }


//============== database_utils tests functions implementation =================
#ifdef ENABLE_TESTS
#endif

//===================== Implement helpful functions ============================

//=============================== Namespace end ================================
}  // namespace rain_text_core

#include "gtest/gtest.h"
#include "utils/rain_text_core_utils.h"

static std::vector<uint8_t> key = {
    0x0f, 0x3c, 0x1c, 0x2e, 0x2e, 0xf4, 0x47, 0x2c, 0xcc, 0x94, 0xcc, 0x37,
    0x4d, 0xfb, 0x5c, 0xbd, 0x25, 0x39, 0xac, 0x38, 0xd0, 0xc6, 0xcb, 0xd4,
    0x5e, 0xc7, 0x14, 0x3d, 0x38, 0x71, 0xae, 0x9d, 0x5b, 0xe8, 0x3a, 0x5e,
    0xd8, 0xbc, 0xc3, 0xd3, 0xb1, 0x9c, 0x3d, 0x4f, 0xb2, 0x91, 0x9f, 0x63,
    0x82, 0x35, 0xd9, 0xcc, 0x4a, 0xa3, 0x6f, 0xee, 0x92, 0x36, 0x8f, 0x64,
    0xd6, 0xc2, 0xf7, 0x2d, 0xee, 0xb8, 0xbf, 0x7a, 0x80, 0x59, 0xd9, 0xd9,
    0xf6, 0xcd, 0xe5, 0x9e, 0x3f, 0x4e, 0x6f, 0xb9, 0x65, 0x5b, 0x24, 0x8a,
    0x0e, 0xe2, 0x15, 0x46, 0x96, 0xac, 0xfb, 0x59, 0x9b, 0x6d, 0xee, 0xd1,
    0xd9, 0xf3, 0xa0, 0x4c, 0x7a, 0x6f, 0x74, 0x6e, 0x06, 0x41, 0x01, 0x8f,
    0xd2, 0xc3, 0x30, 0x53, 0x94, 0xf2, 0x55, 0x69, 0x9f, 0x26, 0x57, 0x29,
    0xb1, 0x92, 0x00, 0x86, 0xcb, 0x10, 0x79, 0x8c, 0xc0, 0x57, 0x75, 0x9d,
    0xe9, 0x97, 0xc7, 0x58, 0xe3, 0xd6, 0x30, 0xaa, 0xd2, 0x1b, 0xb2, 0xde,
    0xcb, 0x71, 0x9b, 0x42, 0xd7, 0xd5, 0xbf, 0xd2, 0x8f, 0x2e, 0x86, 0xca,
    0x88, 0x38, 0x37, 0x1d, 0x3b, 0x3c, 0xbd, 0xe9, 0x00, 0x87, 0xdd, 0xc6,
    0xda, 0x81, 0xf3, 0x24, 0xe8, 0xc3, 0x1d, 0x81, 0x8b, 0x25, 0xa2, 0x97,
    0x17, 0x3c, 0x22, 0x82, 0xe9, 0x8f, 0xc9, 0xe9, 0x6f, 0xd8, 0x55, 0xef,
    0x49, 0x01, 0xed, 0x01, 0xd4, 0xa8, 0xd3, 0x44, 0xc3, 0xbe, 0x00, 0x0c,
    0x10, 0x73, 0x4a, 0xad, 0xee, 0x61, 0x6f, 0x85, 0x4e, 0xc9, 0x36, 0xa2,
    0x5c, 0xcb, 0x13, 0x1c, 0xab, 0xc3, 0x6c, 0x25, 0x2c, 0xd2, 0x66, 0x7c,
    0x9e, 0x15, 0xcd, 0x12, 0x48, 0xd6, 0x1b, 0x10, 0x27, 0xfd, 0x64, 0x93,
    0xa6, 0x34, 0x06, 0xcd, 0xd4, 0x5f, 0xa1, 0x87, 0x31, 0x39, 0x7b, 0xce,
    0x26, 0xb4, 0x07, 0x8c};  // size: 256

TEST(RainTextUtils_SplitKey, AccurateSize) {
  size_t result_size = 8;
  uint16_t result_key_size = 32;
  std::vector<std::vector<uint8_t>> result;
  rain_text_core::rain_text_core_utils::SplitKey(result_key_size, key, result);
  ASSERT_EQ(result_size, result.size());
  for (auto& i : result) {
    ASSERT_EQ(result_key_size, i.size());
  }
}

TEST(RainTextUtils_SplitKey, SmallerSize) {
  size_t result_size = 8;
  uint16_t result_key_size = 31;
  std::vector<std::vector<uint8_t>> result;
  rain_text_core::rain_text_core_utils::SplitKey(result_key_size, key, result);
  ASSERT_EQ(result_size, result.size());
  for (auto& i : result) {
    ASSERT_EQ(result_key_size, i.size());
  }
}

TEST(RainTextUtils_SplitKey, BiggerSize) {
  size_t result_size = 7;
  uint16_t result_key_size = 33;
  std::vector<std::vector<uint8_t>> result;
  rain_text_core::rain_text_core_utils::SplitKey(result_key_size, key, result);
  ASSERT_EQ(result_size, result.size());
  for (auto& i : result) {
    ASSERT_EQ(result_key_size, i.size());
  }
}

TEST(RainTextUtils_GetIV, GetIndexes) {
  uint16_t key_size = 32;
  std::vector<std::vector<uint8_t>> splitedKeys;
  uint8_t result_key_index = 0;
  uint8_t result_iv_index = 0;
  uint8_t result_pre_salt_index = 0;
  uint8_t iv[16];
  rain_text_core::rain_text_core_utils::SplitKey(key_size, key, splitedKeys);

  rain_text_core::rain_text_core_utils::GetIV(splitedKeys, result_key_index,
                                              result_iv_index,
                                              result_pre_salt_index, iv, 16);
  ASSERT_FALSE(result_key_index == result_iv_index);
  ASSERT_FALSE(result_key_index == result_pre_salt_index);
  ASSERT_FALSE(result_pre_salt_index == result_iv_index);
}

TEST(RainTextUtils_GetIV, DecriptIndexes) {
  uint16_t key_size = 32;
  std::vector<std::vector<uint8_t>> splitedKeys;
  uint8_t init_key_index = 0;
  uint8_t init_iv_index = 1;
  uint8_t init_pre_salt_index = 2;
  uint8_t result_key_index = 0;
  uint8_t result_iv_index = 1;
  uint8_t result_pre_salt_index = 2;
  uint8_t iv[16];
  rain_text_core::rain_text_core_utils::SplitKey(key_size, key, splitedKeys);

  rain_text_core::rain_text_core_utils::GetIV(
      splitedKeys, result_key_index, result_iv_index, result_pre_salt_index, iv,
      16, true);

  ASSERT_EQ(init_key_index, result_key_index);
  ASSERT_EQ(init_iv_index, result_iv_index);
  ASSERT_EQ(init_pre_salt_index, result_pre_salt_index);
}

TEST(RainTextUtils_GetIV, EncryptAndDecryptIV) {
  uint16_t key_size = 32;
  std::vector<std::vector<uint8_t>> splitedKeys;
  uint8_t key_index = 0;
  uint8_t iv_index = 0;
  uint8_t pre_salt_index = 0;
  uint8_t encrypt_iv[16];
  uint8_t decrypt_iv[16];
  rain_text_core::rain_text_core_utils::SplitKey(key_size, key, splitedKeys);

  rain_text_core::rain_text_core_utils::GetIV(splitedKeys, key_index, iv_index,
                                              pre_salt_index, encrypt_iv, 16);

  rain_text_core::rain_text_core_utils::GetIV(
      splitedKeys, key_index, iv_index, pre_salt_index, decrypt_iv, 16, true);

  for (int i = 0; i < 16; ++i) {
    ASSERT_EQ(encrypt_iv[i], decrypt_iv[i]);
  }
}

TEST(RainTextUtils_GetIV, EncryptAndDecryptIV10) {
  for (int j = 0; j < 10; ++j) {
    uint16_t key_size = 32;
    std::vector<std::vector<uint8_t>> splitedKeys;
    uint8_t key_index = 0;
    uint8_t iv_index = 0;
    uint8_t pre_salt_index = 0;
    uint8_t encrypt_iv[16];
    uint8_t decrypt_iv[16];
    rain_text_core::rain_text_core_utils::SplitKey(key_size, key, splitedKeys);

    rain_text_core::rain_text_core_utils::GetIV(
        splitedKeys, key_index, iv_index, pre_salt_index, encrypt_iv, 16);

    rain_text_core::rain_text_core_utils::GetIV(
        splitedKeys, key_index, iv_index, pre_salt_index, decrypt_iv, 16, true);

    for (int i = 0; i < 16; ++i) {
      ASSERT_EQ(encrypt_iv[i], decrypt_iv[i]);
    }
  }
}

TEST(RainTextUtils_GetIV, EncryptAndDecryptIV100) {
  for (int j = 0; j < 100; ++j) {
    uint16_t key_size = 32;
    std::vector<std::vector<uint8_t>> splitedKeys;
    uint8_t key_index = 0;
    uint8_t iv_index = 0;
    uint8_t pre_salt_index = 0;
    uint8_t encrypt_iv[16];
    uint8_t decrypt_iv[16];
    rain_text_core::rain_text_core_utils::SplitKey(key_size, key, splitedKeys);

    rain_text_core::rain_text_core_utils::GetIV(
        splitedKeys, key_index, iv_index, pre_salt_index, encrypt_iv, 16);

    rain_text_core::rain_text_core_utils::GetIV(
        splitedKeys, key_index, iv_index, pre_salt_index, decrypt_iv, 16, true);

    for (int i = 0; i < 16; ++i) {
      ASSERT_EQ(encrypt_iv[i], decrypt_iv[i]);
    }
  }
}
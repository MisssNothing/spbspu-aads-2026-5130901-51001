#ifndef SHA1_HPP
#define SHA1_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace petrenko {

class SHA1 {
private:
  uint32_t h0_, h1_, h2_, h3_, h4_;

  void transform(const uint8_t* data) {
    uint32_t w[80];

    for (int i = 0; i < 16; ++i) {
      w[i] = (data[i * 4] << 24) | (data[i * 4 + 1] << 16) |
             (data[i * 4 + 2] << 8) | data[i * 4 + 3];
    }

    for (int i = 16; i < 80; ++i) {
      w[i] = leftRotate(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);
    }

    uint32_t a = h0_, b = h1_, c = h2_, d = h3_, e = h4_;

    for (int i = 0; i < 80; ++i) {
      uint32_t f, k;
      if (i < 20) {
        f = (b & c) | ((~b) & d);
        k = 0x5A827999;
      } else if (i < 40) {
        f = b ^ c ^ d;
        k = 0x6ED9EBA1;
      } else if (i < 60) {
        f = (b & c) | (b & d) | (c & d);
        k = 0x8F1BBCDC;
      } else {
        f = b ^ c ^ d;
        k = 0xCA62C1D6;
      }

      uint32_t temp = leftRotate(a, 5) + f + e + k + w[i];
      e = d;
      d = c;
      c = leftRotate(b, 30);
      b = a;
      a = temp;
    }

    h0_ += a;
    h1_ += b;
    h2_ += c;
    h3_ += d;
    h4_ += e;
  }

  static uint32_t leftRotate(uint32_t value, uint32_t bits) {
    return (value << bits) | (value >> (32 - bits));
  }

  static char hexChar(unsigned int value) {
    return (value < 10) ? ('0' + value) : ('a' + value - 10);
  }

public:
  SHA1() : h0_(0x67452301), h1_(0xEFCDAB89), h2_(0x98BADCFE),
           h3_(0x10325476), h4_(0xC3D2E1F0) {}

  std::string operator()(const std::string& input) const {
    return hashBytes(reinterpret_cast<const uint8_t*>(input.c_str()),
                     input.length());
  }

  std::string hashBytes(const uint8_t* bytes, size_t length) const {
    SHA1 sha;
    std::vector<uint8_t> data(bytes, bytes + length);
    uint64_t bit_len = length * 8;

    data.push_back(0x80);

    while ((data.size() % 64) != 56) {
      data.push_back(0);
    }

    for (int i = 7; i >= 0; --i) {
      data.push_back((bit_len >> (i * 8)) & 0xFF);
    }

    for (size_t i = 0; i < data.size(); i += 64) {
      sha.transform(data.data() + i);
    }

    std::string result;
    result.reserve(40);

    uint32_t values[] = {sha.h0_, sha.h1_, sha.h2_, sha.h3_, sha.h4_};
    for (uint32_t val : values) {
      for (int j = 28; j >= 0; j -= 4) {
        result.push_back(hexChar((val >> j) & 0xF));
      }
    }

    return result;
  }
};

size_t byteHash(const uint8_t* bytes, size_t length) {
  SHA1 sha;
  std::string hash_str = sha.hashBytes(bytes, length);
  return std::hash<std::string>{}(hash_str);
}

}

#endif

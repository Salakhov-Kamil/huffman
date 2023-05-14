#pragma once

#include "Node.h"
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

namespace BitStream {
using bit = bool;
using bits = std::vector<bit>;

constexpr unsigned char BITS_IN_BYTE = 8;
const constexpr std::size_t BITS_IN_CHAR = BITS_IN_BYTE * sizeof(char);

struct bitReader {
public:
  explicit bitReader(std::istream &);
  bit readBit();

  void refresh();

  unsigned char readChar();

  template <typename T> T read() {
    T result = 0;
    for (std::size_t i = 0; i < BITS_IN_BYTE * sizeof(T); ++i) {
      result |= (static_cast<unsigned long long>(readBit()) << i);
    }
    return result;
  }

  [[nodiscard]] bool eof();

private:
  static const constexpr std::size_t BUFF_SIZE = 1 << 15;
  unsigned char buff[BUFF_SIZE];
//  static const constexpr std::size_t BITS_IN_BUFF = BITS_IN_CHAR * sizeof(buff);
  std::size_t pos = 0;
  std::size_t cntReadChars = 0;
  std::istream &in;
  void fillBuff();
};

struct bitWriter {
public:
  explicit bitWriter(std::ostream &);

  void writeBit(bit);
  void write(const bits &);
  void writeChar(unsigned char c);

  template <typename T> void write(const T &num) {
    BitStream::bits bs;
    for (size_t i = 0; i < BITS_IN_BYTE * sizeof(T); ++i) {
      writeBit(static_cast<bit>(num & (1ll << i)));
    }
  }

  void close();

private:
  static const constexpr std::size_t BUFF_SIZE = 1 << 15;
  unsigned char buff[BUFF_SIZE];
  static const constexpr std::size_t BITS_IN_BUFF = BITS_IN_CHAR * sizeof(buff);
  std::size_t pos = 0;
  std::ostream &out;
};
} // namespace BitStream

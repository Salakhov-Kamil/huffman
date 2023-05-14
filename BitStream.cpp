//
// Created by Salakhov Kamil on 03.09.2022.
//

#include "BitStream.h"
#include <cstring>
#include <iostream>

BitStream::bitReader::bitReader(std::istream &in) : in(in) {
  std::fill(buff, buff + BUFF_SIZE, 0);
  //  std::memset(buff, 0, sizeof(buff));
}

BitStream::bit BitStream::bitReader::readBit() {
  if (eof()) {
    throw std::out_of_range("Trying to read, when nothing to read.");
  }

  bit result = buff[pos / BITS_IN_CHAR] & (1 << (pos % BITS_IN_CHAR));
  ++pos;
  return result;
}

bool BitStream::bitReader::eof() {
  fillBuff();
  return pos == cntReadChars * BITS_IN_CHAR && in.eof();
}

void BitStream::bitReader::fillBuff() {
  if (pos == cntReadChars * BITS_IN_CHAR) {
    in.read(reinterpret_cast<char *>(buff), BUFF_SIZE);
    cntReadChars = in.gcount();
    pos = 0;
  }
}

void BitStream::bitReader::refresh() {
  in.clear();
  in.seekg(0);
  pos = 0;
  cntReadChars = 0;
  std::memset(buff, 0, sizeof(buff));
}

unsigned char BitStream::bitReader::readChar() {
  if (eof()) {
    throw std::out_of_range("Trying to read, when nothing to read.");
  }

  assert(pos % BITS_IN_CHAR == 0);
  unsigned char result = buff[pos / BITS_IN_CHAR];
  pos += BITS_IN_CHAR;

  return result;
}

BitStream::bitWriter::bitWriter(std::ostream &out) : out(out) {
  std::fill(buff, buff + BUFF_SIZE, 0);
}

void BitStream::bitWriter::writeBit(BitStream::bit b) {
  buff[pos / BITS_IN_CHAR] |= (b << (pos % BITS_IN_CHAR));
  ++pos;

  if (pos == BITS_IN_BUFF) {
    out.write(reinterpret_cast<char *>(buff), BUFF_SIZE);
    pos = 0;
    std::memset(buff, 0, sizeof(buff));
  }
}

void BitStream::bitWriter::close() {
  if (pos) {
    out.write(reinterpret_cast<char *>(buff),
              (pos + BITS_IN_CHAR - 1) / BITS_IN_CHAR);
  }
  out.flush();
}

void BitStream::bitWriter::write(const bits &bs) {
  for (auto b : bs) {
    writeBit(b);
  }
}

void BitStream::bitWriter::writeChar(unsigned char c) {
  assert(pos % BITS_IN_CHAR == 0);
  buff[pos / BITS_IN_CHAR] = c;
  pos += BITS_IN_CHAR;

  if (pos == BITS_IN_BUFF) {
    out.write(reinterpret_cast<char *>(buff), BUFF_SIZE);
    pos = 0;
    std::memset(buff, 0, sizeof(buff));
  }
}

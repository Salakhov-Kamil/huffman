#pragma once

#include "BitStream.h"
#include "Coder.h"
#include "Node.h"
#include <map>

namespace huffman {
class Decoder {
public:
  Decoder(std::istream &, std::ostream &);
  void decode();

private:
  BitStream::bitReader reader;
  BitStream::bitWriter writer;
  std::size_t textLength;

  Node *decodeHuffmanTree();

  void decodeText(Node *);
};
} // namespace huffman

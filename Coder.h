#pragma once

#include "BitStream.h"
#include "Node.h"
#include <istream>
#include <limits>
#include <map>

namespace huffman {
class Coder {
public:
  void encode();

private:
  static const constexpr std::size_t MAX_CHAR =
      std::numeric_limits<unsigned char>::max() + 1;

  BitStream::bitReader reader;
  BitStream::bitWriter writer;
  std::size_t cntChar[MAX_CHAR];
  BitStream::bits huffmanTable[MAX_CHAR];

  huffman::Node *makeHuffmanTree();

  void codeHuffmanTree(const Node *);

  void writeText();

  [[nodiscard]] std::size_t textSize();

  void fillHuffmanTable(Node *);

  void fillHuffmanTable(Node *, BitStream::bits &code);

public:
  Coder(std::istream &, std::ostream &);
};
} // namespace huffman
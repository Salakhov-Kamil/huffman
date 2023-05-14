#include <iostream>
// #include "lib.h"
#include "../BitStream.h"
#include "../Coder.h"
#include "../Decoder.h"
#include <gtest/gtest.h>
#include <sstream>
#include <vector>

TEST(BitStream, equalInputOtput) {
  BitStream::bits bs(
      {1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0});

  std::stringstream out;
  BitStream::bitWriter writer(out);
  writer.write(bs);
  writer.close();

  BitStream::bitReader reader(out);
  BitStream::bits bs1;
  for (std::size_t i = 0; i < bs.size(); ++i) {
    bs1.push_back(reader.readBit());
  }

  EXPECT_EQ(bs, bs1);
}

void EQ_CODE_DECODE(const std::string& test) {
  std::stringstream in(test);
  std::stringstream out;
  huffman::Coder coder(in, out);
  coder.encode();
  std::stringstream result;
  huffman::Decoder decoder(out, result);
  decoder.decode();
  //  std::cout << result.str() << '\n';
  EXPECT_EQ(test, result.str());
}

TEST(CodeDecode, emptyTest) {
  const std::string test = "";
  EQ_CODE_DECODE(test);
}

TEST(CodeDecode, oneLetterTest) {
  const std::string test = "z";
  EQ_CODE_DECODE(test);
}

TEST(CodeDecode, smallTest) {
  const std::string test = "abacaba";
  EQ_CODE_DECODE(test);
}

TEST(CodeDecode, allSymbolsTest) {
  const std::string test =
      "1234567890qwertyuiop[]asdfghjkl;'\\zxcvbnm,./~!@#$%^&*()_+-=";
  EQ_CODE_DECODE(test);
}

TEST(CodeDecode, sameLetter) {
  const std::string test = "gggggggggggggggggggggggggggggggggggggggggggggggggg";
  EQ_CODE_DECODE(test);
}

TEST(CodeDecode, bigTestTemplate) {
  const std::size_t CNT_ITERATIONS = 10000;
  const std::string temp = "asdfqwerr";
  std::string test = "";
  for (std::size_t i = 0; i < CNT_ITERATIONS; ++i) {
    test += temp;
  }
  EQ_CODE_DECODE(test);
}

TEST(CodeDecode, bigTestDifferentLetters) {
  const std::size_t CNT_ITERATIONS = 10000;
  std::string test = "";
  for (std::size_t i = 0; i < CNT_ITERATIONS; ++i) {
    test.push_back('a' + (i % 26));
  }
  EQ_CODE_DECODE(test);
}







// TEST(HuffmanTree, equalHuffmanTable) {
//     const std::string test = "0123456789abcdefghijklmnopqrstuvwxyz";
//     std::stringstream in(test);
//
//     std::stringstream out;
//     huffman::Coder coder(in, out);
//     coder.writeHuffmanTree();
//
//     std::stringstream result;
//     huffman::Decoder decoder(out, result);
//     huffman::Node* node = decoder.decodeHuffmanTree();
//     std::map<unsigned char, BitStream::bits> huffmanTable;
//     BitStream::bits code;
//     decoder.fillHuffmanTable(node, huffmanTable, code);
//
//     std::string rr;
//     for (auto x : huffmanTable) {
//         rr += x.first;
//     }
//     EXPECT_EQ(test, rr);
// }
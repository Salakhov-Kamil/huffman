//
// Created by Salakhov Kamil on 03.09.2022.
//

#include "Coder.h"
#include "BitStream.h"
#include <cassert>
#include <cstring>
#include <map>
#include <queue>
#include <set>

huffman::Coder::Coder(std::istream &in, std::ostream &out)
    : reader(in), writer(out) {}

huffman::Node *huffman::Coder::makeHuffmanTree() {
  std::memset(cntChar, 0, sizeof(cntChar));

  while (!reader.eof()) {
    ++cntChar[reader.readChar()];
  }

  ++cntChar[static_cast<unsigned char>('a')];
  ++cntChar[static_cast<unsigned char>('b')];

  using pairType = std::pair<std::size_t, huffman::Node *>;
  std::priority_queue<pairType, std::vector<pairType>, std::greater<>> nodes;

  for (std::size_t i = 0; i < MAX_CHAR; ++i) {
    if (cntChar[i]) {
      nodes.push({cntChar[i], new huffman::LeafNode(i)});
    }
  }

  while (nodes.size() > 1) {
    pairType first = nodes.top();
    nodes.pop();
    pairType second = nodes.top();
    nodes.pop();
    nodes.push({first.first + second.first,
                new huffman::InnerNode(first.second, second.second)});
  }

  --cntChar[static_cast<unsigned char>('a')];
  --cntChar[static_cast<unsigned char>('b')];

  return (nodes.top()).second;
}

void huffman::Coder::codeHuffmanTree(const huffman::Node *node) {
  if (node->type == Node::NodeType::Inner) {
    writer.writeBit(0);
    auto innerNode = static_cast<const InnerNode *>(node);
    assert(innerNode->left && innerNode->right);
    codeHuffmanTree(innerNode->left);
    codeHuffmanTree(innerNode->right);
  }
  if (node->type == Node::NodeType::Leaf) {
    writer.writeBit(1);
    auto leafNode = static_cast<const LeafNode *>(node);
    writer.write(leafNode->c);
  }
}

void huffman::Coder::encode() {
  Node *huffmanTree = makeHuffmanTree();
  codeHuffmanTree(huffmanTree);
  fillHuffmanTable(huffmanTree);
  writer.write(textSize());
  writeText();
  writer.close();

  assert(huffmanTree->type == Node::NodeType::Inner);
  delete static_cast<InnerNode *>(huffmanTree);
}

std::size_t huffman::Coder::textSize() {
  std::size_t textLength = 0;

  for (std::size_t i = 0; i < MAX_CHAR; ++i) {
    textLength += cntChar[i] * huffmanTable[i].size();
  }

  return textLength;
}

void huffman::Coder::writeText() {
  reader.refresh();

  while (!reader.eof()) {
    writer.write(huffmanTable[reader.readChar()]);
  }
}

void huffman::Coder::fillHuffmanTable(Node *root) {
  BitStream::bits code;
  fillHuffmanTable(root, code);
}

void huffman::Coder::fillHuffmanTable(Node *root, BitStream::bits &code) {
  switch (root->type) {
  case Node::NodeType::Inner:
    code.push_back(0);
    fillHuffmanTable(static_cast<InnerNode *>(root)->left, code);
    code.pop_back();
    code.push_back(1);
    fillHuffmanTable(static_cast<InnerNode *>(root)->right, code);
    code.pop_back();
    break;
  case Node::NodeType::Leaf:
    huffmanTable[static_cast<LeafNode *>(root)->c] = code;
    break;
  default:
    break;
  }
}
//
// Created by Salakhov Kamil on 03.09.2022.
//

#include "Decoder.h"
#include <cassert>
#include <iostream>

huffman::Decoder::Decoder(std::istream &in, std::ostream &out)
    : reader(in), writer(out), textLength(0) {}

huffman::Node *huffman::Decoder::decodeHuffmanTree() {
  if (!reader.readBit()) {
    auto leftSon = decodeHuffmanTree();
    auto rightSon = decodeHuffmanTree();
    return new huffman::InnerNode(leftSon, rightSon);
  } else {
    return new huffman::LeafNode(reader.read<unsigned char>());
  }
}

void huffman::Decoder::decode() {
  Node *hufTree = decodeHuffmanTree();
  textLength = reader.read<std::size_t>();
  decodeText(hufTree);
  writer.close();

  assert(hufTree->type == Node::NodeType::Inner);
  delete static_cast<InnerNode *>(hufTree);
}

void huffman::Decoder::decodeText(Node *hufTree) {
  Node *node = hufTree;
  while (textLength--) {
    BitStream::bit b = reader.readBit();

    assert(node->type == Node::NodeType::Inner);

    if (!b) {
      node = static_cast<InnerNode *>(node)->left;
    } else {
      node = static_cast<InnerNode *>(node)->right;
    }

    if (node->type == Node::NodeType::Leaf) {
      writer.writeChar(static_cast<LeafNode *>(node)->c);
      node = hufTree;
    }
  }
}
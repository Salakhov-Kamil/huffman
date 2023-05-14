//
// Created by Salakhov Kamil on 03.09.2022.
//

#include "Node.h"
void huffman::InnerNode::myDelete(Node *node) {
  assert(node);
  if (node->type == Node::NodeType::Inner) {
    delete static_cast<InnerNode *>(node);
  } else {
    assert(node->type == Node::NodeType::Leaf);
    delete static_cast<LeafNode *>(node);
  }
}

huffman::InnerNode::~InnerNode() {
  myDelete(left);
  myDelete(right);
}

huffman::InnerNode::InnerNode(huffman::Node *left, huffman::Node *right)
    : Node(NodeType::Inner), left(left), right(right) {}

huffman::LeafNode::LeafNode(unsigned char c) : Node(NodeType::Leaf), c(c) {}

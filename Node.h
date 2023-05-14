#pragma once

#include <cassert>

namespace huffman {
struct Node {
  enum class NodeType { Leaf, Inner };

  NodeType type;

  explicit Node(NodeType type) : type(type) {}
};

struct InnerNode : Node {
  Node *left;
  Node *right;

  InnerNode(Node *left, Node *right);

  ~InnerNode();

private:
  void myDelete(Node *);
};

struct LeafNode : Node {
  unsigned char c;

  explicit LeafNode(unsigned char c);
};
} // namespace huffman

#ifndef LINK_H
#define LINK_H

#include "object.h"
#include "packet.h"
#include <cstdlib>

class Node;

class Link : public Object {
  friend class LinkInstaller;

private:
  Link(Node *nodeA, Node *nodeB, double delay = 0.0)
      : nodeA_(nodeA), nodeB_(nodeB), delay_(delay) {}

  Node *nodeA_;
  Node *nodeB_;

  Node *other(const Node *node) const {
    return node == nodeA_ ? nodeB_ : nodeA_;
  }

  double delay_ = 0.0;
  std::string name() override { return "Link"; }

public:
  Node *a() const { return nodeA_; }
  Node *b() const { return nodeB_; }
  virtual void onReceive(Node *node, Packet *packet);
  double delay() const { return delay_; }
};

#endif
#ifndef NODE_H
#define NODE_H

#include "address.h"
#include "link.h"
#include "object.h"
#include "packet.h"
#include <iostream>
#include <set>
#include <vector>

class Node : public Object {
  friend class LinkInstaller;

private:
  void addLink(Link *link) { links_.push_back(link); }
  virtual std::string name() override { return "Node"; }

public:
  virtual void onReceive(Link *link, Packet *packet) = 0;

  virtual bool hasAddress() { return false; }

  virtual Address address() {
    throw std::runtime_error("this node does not have address");
  }

protected:
  std::vector<Link *> links_;
};

#endif
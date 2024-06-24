#ifndef ROUTER_H
#define ROUTER_H

#include "node.h"
#include <iostream>

struct RoutingEntry {
public:
  Address destination;
  Link *nextLink;
};

class Router : public Node {
private:
  virtual std::string name() override { return "Router"; }

protected:
  std::vector<RoutingEntry> routingTable_;

public:
  virtual ~Router() {}
  void onReceive(Link *link, Packet *packet) override {
    for (RoutingEntry entry : routingTable_) {
      if (entry.destination == packet->destAddress()) {
        log("forwarding packet: " + packet->toString() + " to " +
            entry.nextLink->toString());
        entry.nextLink->onReceive(this, packet);
        return;
      }
    }

    delete packet;
  }
};

#endif
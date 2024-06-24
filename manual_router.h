#ifndef MANUAL_ROUTER_H
#define MANUAL_ROUTER_H

#include "address.h"
#include "router.h"

class ManualRouter : public Router {
private:
  std::string name() override { return "ManualRouter"; }

public:
  void addRoutingEntry(const Address &destination, Link *nextLink) {
    routingTable_.push_back({destination, nextLink});
  }
};

#endif
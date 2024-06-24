#ifndef FIREWALL_H
#define FIREWALL_H

#include "address.h"
#include "node.h"
#include <set>
#include <string>

class Firewall : public Node {
private:
  class Policy {
  public:
    virtual bool isAllowed(Packet *packet) = 0;
    virtual ~Policy() {}
  };

  class AllowAnyPolicy : public Policy {
  private:
    short destPort_;

  public:
    AllowAnyPolicy(short destPort) : destPort_(destPort) {}

    bool isAllowed(Packet *packet) override {
      return packet->destPort() == destPort_;
    }
  };

  class AllowSpecificPolicy : public Policy {
  private:
    Address srcAddress_;
    short destPort_;

  public:
    AllowSpecificPolicy(Address srcAddress, short destPort)
        : srcAddress_(srcAddress), destPort_(destPort) {}
    bool isAllowed(Packet *packet) override {
      return packet->srcAddress() == srcAddress_ &&
             packet->destPort() == destPort_;
    }
  };

  std::vector<Policy *> policies_;
  Link *receiveLink_ = nullptr;

  bool isPacketAllowed(Packet *packet) {
    for (auto policy : policies_) {
      if (policy->isAllowed(packet)) {
        return true;
      }
    }
    return false;
  }

  bool isAllowed(Link *link, Packet *packet) {
    return link != receiveLink_ || isPacketAllowed(packet);
  }

  std::string name() override { return "Firewall"; }

public:
  void initialize() override {
    if (receiveLink_ == nullptr) {
      throw std::runtime_error("Receive link is not set");
    }

    if (links_.size() != 2) {
      throw std::runtime_error("Firewall must have exactly 2 links");
    }

    bool found = false;
    for (auto link : links_) {
      if (link == receiveLink_) {
        found = true;
        break;
      }
    }

    if (!found) {
      throw std::runtime_error("Receive link is not connected to the firewall");
    }
  }

  void setReceiveLink(Link *link) { receiveLink_ = link; }

  void addAllowAnyPolicy(short destPort) {
    policies_.push_back(new AllowAnyPolicy(destPort));
  }

  void addAllowSpecificPolicy(Address srcAddress, short destPort) {
    policies_.push_back(new AllowSpecificPolicy(srcAddress, destPort));
  }

  void onReceive(Link *link, Packet *packet) override {
    if (isAllowed(link, packet)) {
      int linkIndex = links_[0] == link ? 1 : 0;
      Link *link = links_[linkIndex];
      link->onReceive(this, packet);
    } else {
      log("Dropping " + packet->toString() + " with src address " +
          packet->srcAddress().toString() + " and dest port " +
          std::to_string(packet->destPort()));
      delete packet;
    }
  }

  ~Firewall() {
    for (auto policy : policies_) {
      delete policy;
    }
  }
};

#endif
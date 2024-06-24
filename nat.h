#ifndef NAT_H
#define NAT_H

#include "address.h"
#include "host.h"
#include "link.h"
#include <set>
#include <vector>

struct NatEntry {
  Address internalAddress;
  short internalPort;
  short externalPort;
};

class Nat : public Node {
private:
  Address address_;
  std::string name() override { return "Nat"; }

  Link *wanLink_;

  std::vector<NatEntry> natTable_;
  std::set<short> usedPorts_;

  void translateOutbound(Packet *&packet) {
    auto entry =
        findNatEntryByInternal(packet->srcAddress(), packet->srcPort());
    if (entry != nullptr) {
      auto newPacket =
          new Packet(address_, packet->destAddress(), entry->externalPort,
                     packet->destPort(), packet->data());
      auto temp = packet;
      packet = newPacket;
      delete temp;
    } else {
      NatEntry newEntry{packet->srcAddress(), packet->srcPort(),
                        availablePort()};
      natTable_.push_back(newEntry);
      translateOutbound(packet);
    }
  }

  void translateInbound(Packet *&packet) {
    auto entry = findNatEntryByExternal(packet->destPort());
    if (entry != nullptr) {
      auto newPacket =
          new Packet(packet->srcAddress(), entry->internalAddress,
                     packet->srcPort(), entry->internalPort, packet->data());
      auto temp = packet;
      packet = newPacket;
      delete temp;
    } else {
      log("no nat entry for packet: " + packet->toString());
      delete packet;
      packet = nullptr;
    }
  }

  NatEntry *findNatEntryByInternal(Address address, short port) {
    for (NatEntry &entry : natTable_) {
      if (entry.internalAddress == address && entry.internalPort == port) {
        return &entry;
      }
    }
    return nullptr;
  }

  NatEntry *findNatEntryByExternal(short port) {
    for (NatEntry &entry : natTable_) {
      if (entry.externalPort == port) {
        return &entry;
      }
    }
    return nullptr;
  }

  short availablePort() {
    for (unsigned short port = 1000; port <= 65535; port++) {
      if (usedPorts_.find(port) == usedPorts_.end()) {
        usedPorts_.insert(port);
        return port;
      }
    }
    throw std::runtime_error("No available port");
  }

public:
  Nat(Address address) : address_(address) {}

  bool hasAddress() override { return true; }
  Address address() override { return address_; }

  void setWanLink(Link *link) { wanLink_ = link; }

  void initialize() override {
    if (wanLink_ == nullptr) {
      throw std::runtime_error("wanLink is not set");
    }

    auto found = false;
    for (Link *l : links_) {
      if (l == wanLink_) {
        found = true;
        break;
      }
    }

    if (!found) {
      throw std::runtime_error("wanLink is not in links");
    }
  }

  void onReceive(Link *link, Packet *packet) override {
    if (link == wanLink_) {
      translateInbound(packet);
      if (packet != nullptr) {
        for (Link *l : links_) {
          if (l != link) {
            l->onReceive(this, new Packet(*packet));
          }
        }
        delete packet;
      }
    } else {
      translateOutbound(packet);
      wanLink_->onReceive(this, packet);
    }
  }
};

#endif
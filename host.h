#ifndef HOST_H
#define HOST_H

#include "address.h"
#include "packet.h"
#include "service.h"
#include <vector>

class Host : public Node {
  friend class ServiceInstaller;

private:
  Address address_;
  std::vector<Service *> services_;
  std::string name() override { return "Host"; }

public:
  Address address() override { return address_; }

  bool hasAddress() override { return true; }

  Host(Address address) : address_(address) {}

  short availablePort() {
    for (unsigned short port = 1000; port <= 65535; port++) {
      bool isAvailable = true;
      for (Service *service : services_) {
        if (service->port() == port) {
          isAvailable = false;
          break;
        }
      }
      if (isAvailable) {
        return port;
      }
    }
    throw std::runtime_error("No available port");
  }

  void onReceive(Link *link, Packet *packet) override {
    if (!(packet->destAddress() == address_)) {
      delete packet;
      return;
    }

    for (Service *service : services_) {
      if (service->port() == packet->destPort()) {
        log("received packet: " + packet->toString() + ", forwarding to " +
            service->toString());
        service->onReceive(packet);
        delete packet;
        return;
      }
    }

    log("no service for packet: " + packet->toString());
    delete packet;
  }

  void send(Service *service, Address destAddress, short destPort,
            const std::vector<char> &data) {
    Packet *packet =
        new Packet(address_, destAddress, service->port(), destPort, data);
    int linkIndex = rand() % links_.size();
    log("sending packet: " + packet->toString());

    // send to random link
    links_[linkIndex]->onReceive(this, packet);
  }
};

#endif
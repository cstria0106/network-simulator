#ifndef PACKET_SINK_SERVICE_H
#define PACKET_SINK_SERVICE_H

#include "service.h"
#include <string>

class PacketSinkService : public Service {
private:
  int totalBytesReceived = 0;
  std::string name() override { return "PacketSinkService"; }

public:
  void initialize() override {}
  void onReceive(Packet *packet) override {
    log("received total " +
        std::to_string(totalBytesReceived += packet->data().size()) + " bytes");
  }
  PacketSinkService(Host *host, short port) : Service(host, port) {}
};

#endif
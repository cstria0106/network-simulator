#ifndef ECHO_SERVICE_H
#define ECHO_SERVICE_H

#include "host.h"
#include "service.h"
#include <iostream>
#include <string>

// 수신한 패킷을 전송자에게 다시 전송하는 서비스
class EchoService : public Service {
  friend class EchoServiceInstaller;

public:
  void initialize() override {}

  void onReceive(Packet *packet) override {
    std::string data =
        std::string(packet->data().begin(), packet->data().end());
    log("received \"" + data + "\" from " + packet->srcAddress().toString() +
        ":" + std::to_string(packet->srcPort()) +
        ", send reply with same data");
    send(packet->srcAddress(), packet->srcPort(), packet->data());
  }

private:
  EchoService(Host *host, short port) : Service(host, port) {}

  std::string name() override { return "EchoService"; }
};

#endif
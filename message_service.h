#ifndef MESSAGE_SERVICE_H
#define MESSAGE_SERVICE_H

#include "address.h"
#include "host.h"
#include "service.h"
#include <iostream>

// send 함수를 호출하여 메시지를 전송할 수 있는 서비스
class MessageService : public Service {
  friend class MessageServiceInstaller;

private:
  // 목적지 주소
  Address destAddress_;
  // 목적지 포트
  short destPort_;
  MessageService(Host *host, short port, Address destAddress, short destPort)
      : Service(host, port), destAddress_(destAddress), destPort_(destPort) {}

  std::string name() override { return "MessageService"; }

public:
  // 메시지를 전송한다
  void send(std::string message) {
    Service::send(destAddress_, destPort_, message);
  }
  void initialize() override {}

  void onReceive(Packet *packet) override {
    std::string data =
        std::string(packet->data().begin(), packet->data().end());
    log("received \"" + data + "\" from " + packet->srcAddress().toString() +
        ":" + std::to_string(packet->srcPort()));
  }
};

#endif
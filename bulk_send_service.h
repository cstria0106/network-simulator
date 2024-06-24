#ifndef BULK_SEND_SERVICE_H
#define BULK_SEND_SERVICE_H

#include "host.h"
#include "service.h"
#include "simulator.h"

class BulkSendService : Service {
  friend class BulkSendServiceInstaller;

public:
  void initialize() override {
    Simulator::schedule(startTime_, [this]() { send(); });
  }

  void onReceive(Packet *packet) override {}

private:
  Host *host_;
  Address destination_;
  short port_;
  double delay_;
  double startTime_;
  double stopTime_;

  BulkSendService(Host *host, Address destination, short port, double delay = 1,
                  double startTime = 0, double stopTime = 10.0)
      : Service(host, host->availablePort()), host_(host),
        destination_(destination), port_(port), delay_(delay),
        startTime_(startTime), stopTime_(stopTime) {}

  std::string name() override { return "BulkSendService"; }

  void send() {
    std::vector<char> data = std::vector<char>(512, 'A');
    log("sending data");
    Service::send(destination_, port_, data);

    // schedule next send
    auto nextTime = Simulator::now() + delay_;
    if (nextTime < stopTime_) {
      Simulator::schedule(nextTime, [this]() { send(); });
    }
  }
};

#endif
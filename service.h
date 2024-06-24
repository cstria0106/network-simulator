#ifndef SERVICE_H
#define SERVICE_H

#include "address.h"
#include "node.h"
#include "object.h"
#include <vector>

class Host;

class Service : public Object {
  friend class ServiceInstaller;

protected:
  Host *host_;
  short port_;
  Service(Host *host, int port) : host_(host), port_(port) {}

  void send(Address address, short port, const std::vector<char> &data);
  void send(Address address, short port, const std::string &data);

public:
  virtual ~Service() {}
  virtual void initialize() = 0;
  virtual void onReceive(Packet *packet) = 0;
  short port() { return port_; }
};

#endif
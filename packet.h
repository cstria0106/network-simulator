#ifndef PACKET_H
#define PACKET_H

#include "address.h"
#include "object.h"
#include <string>
#include <vector>

class Packet : public Object {
public:
  Packet(Address srcAddress, Address destAddress, short srcPort, short destPort,
         std::vector<char> data)
      : srcAddress_(srcAddress), destAddress_(destAddress), srcPort_(srcPort),
        destPort_(destPort), data_(data) {}

  Address srcAddress() { return srcAddress_; }
  Address destAddress() { return destAddress_; }
  short srcPort() { return srcPort_; }
  short destPort() { return destPort_; }
  const std::vector<char> &data() { return data_; }

private:
  Address srcAddress_;
  Address destAddress_;
  short srcPort_;
  short destPort_;
  std::vector<char> data_;
  std::string name() override { return "Packet"; }
};

#endif
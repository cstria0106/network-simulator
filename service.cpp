#include "service.h"
#include "host.h"

void Service::send(Address address, short port, const std::vector<char> &data) {
  host_->send(this, address, port, data);
}

void Service::send(Address address, short port, const std::string &string) {
  std::vector<char> data(string.begin(), string.end());
  host_->send(this, address, port, data);
}
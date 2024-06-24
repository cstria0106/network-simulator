#ifndef SERVICE_INSTALLER_H
#define SERVICE_INSTALLER_H

#include "host.h"
#include "service.h"

class ServiceInstaller {
protected:
  void installService(Host *host, Service *service) {
    service->host_ = host;
    host->services_.push_back(service);
  }
};

#endif
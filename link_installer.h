#ifndef LINK_INSTALLER_H
#define LINK_INSTALLER_H

#include "link.h"
#include "service.h"

class LinkInstaller {
public:
  Link *install(Node *a, Node *b, double delay = 0.0) {
    Link *link = new Link(a, b, delay);
    a->links_.push_back(link);
    b->links_.push_back(link);
    return link;
  }
};

#endif

#include "link.h"
#include "node.h"
#include "simulator.h"

void Link::onReceive(Node *node, Packet *packet) {
  log("packet in: " + packet->toString() + " from " + node->toString());
  Simulator::schedule(Simulator::now() + delay_, [this, node, packet]() {
    log("packet out: " + packet->toString() + " to " + other(node)->toString());
    other(node)->onReceive(this, packet);
  });
}

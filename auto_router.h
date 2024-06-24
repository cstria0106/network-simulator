#ifndef AUTO_ROUTER_H
#define AUTO_ROUTER_H

#include "link.h"
#include "router.h"
#include "service.h"
#include <algorithm>
#include <limits>
#include <map>
#include <queue>
#include <set>
#include <vector>

class AutoRouter : public Router {
private:
  std::string name() override { return "AutoRouter"; }

public:
  void calculate(const std::vector<Node *> &nodes,
                 const std::vector<Link *> &links) {
    std::map<Node *, int> nodeToIndex;

    // set distances
    std::vector<std::vector<double>> distances(
        nodes.size(),
        std::vector<double>(nodes.size(),
                            std::numeric_limits<double>::infinity()));

    for (size_t i = 0; i < nodes.size(); ++i) {
      distances[i][i] = 0;
      nodeToIndex[nodes[i]] = i;
    }

    for (Link *link : links) {
      int indexA = nodeToIndex[link->a()];
      int indexB = nodeToIndex[link->b()];
      distances[indexA][indexB] = link->delay();
      distances[indexB][indexA] = link->delay();
    }

    int sourceIndex = nodeToIndex[this];

    // Dijkstra's algorithm
    std::vector<double> minDistances(nodes.size(),
                                     std::numeric_limits<double>::infinity());
    std::vector<int> previous(nodes.size(), -1);
    minDistances[sourceIndex] = 0;

    std::priority_queue<std::pair<double, int>,
                        std::vector<std::pair<double, int>>,
                        std::greater<std::pair<double, int>>>
        pq;
    pq.push({0, sourceIndex});

    while (!pq.empty()) {
      int u = pq.top().second;
      pq.pop();

      for (size_t v = 0; v < nodes.size(); ++v) {
        if (distances[u][v] < std::numeric_limits<double>::infinity()) {
          double alt = minDistances[u] + distances[u][v];
          if (alt < minDistances[v]) {
            minDistances[v] = alt;
            previous[v] = u;
            pq.push({alt, v});
          }
        }
      }
    }

    // setup routing table
    routingTable_.clear();

    for (auto n : nodes) {
      if (n->hasAddress()) {
        int destIndex = nodeToIndex[n];
        if (destIndex != sourceIndex && previous[destIndex] != -1) {
          int nextHopIndex = destIndex;
          while (previous[nextHopIndex] != sourceIndex) {
            nextHopIndex = previous[nextHopIndex];
          }
          Address destination = n->address();
          Link *nextLink = findLink(this, nodes[nextHopIndex], links);
          if (nextLink) {
            routingTable_.push_back({destination, nextLink});
          }
        }
      }
    }
  }

private:
  static Link *findLink(Node *from, Node *to,
                        const std::vector<Link *> &links) {
    for (Link *link : links) {
      if ((link->a() == from && link->b() == to) ||
          (link->a() == to && link->b() == from)) {
        return link;
      }
    }
    return nullptr;
  }
};

#endif
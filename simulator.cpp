#include "simulator.h"
#include "object.h"

std::priority_queue<Schedule> Simulator::schedules_ =
    std::priority_queue<Schedule>();

double Simulator::time_ = 0.0;

void Simulator::run() {
  Object::initializeAll();
  while (!schedules_.empty()) {
    Schedule schedule = schedules_.top();
    schedules_.pop();
    time_ = schedule.time();
    schedule.call();
  }
  Object::cleanup();
}
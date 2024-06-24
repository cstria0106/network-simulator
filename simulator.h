#ifndef SIMULATOR_H
#define SIMULATOR_H

#define RANDOM_SEED 369369

#include <cstdlib>
#include <functional>
#include <queue>
#include <vector>

class Simulator;

class Schedule {
private:
  double time_;
  std::function<void()> function_;

public:
  double time() { return time_; }
  void call() { function_(); }

  bool operator<(const Schedule &rhs) const { return time_ > rhs.time_; }

  Schedule(double time, std::function<void()> function)
      : time_(time), function_(function) {}
};

class Simulator {
private:
  static std::priority_queue<Schedule> schedules_;
  static double time_;

public:
  static double now() { return time_; }

  static void schedule(double time, std::function<void()> function) {
    schedules_.push(Schedule(time, function));
  }

  static void prepare() { srand(RANDOM_SEED); }

  static void run();
};

#endif
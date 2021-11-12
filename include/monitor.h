#pragma once

#include <collatz.h>

#include <iostream>
#include <chrono>
#include <stdint.h>
#include <thread>
#include <mutex>

struct Segment {
  Segment(uint64_t start, uint64_t end) : start(start), end(end) {}
  uint64_t start;
  uint64_t end;
};

class Monitor {
public:
  Monitor(int num_threads, uint64_t checks, uint64_t segment_size) : num_threads_m(num_threads), checks_m(checks), segment_size_m(segment_size),
    checked_m(0), counter_control_m(0), done_m(false)
  {}
  void Run();

private:
  int num_threads_m;
  uint64_t checks_m;
  uint64_t segment_size_m;
  uint64_t checked_m;
  bool done_m;

  std::chrono::high_resolution_clock::time_point start_time_m;

  uint64_t counter_control_m;
  std::mutex provider_mutex_m;
  std::mutex updater_mutex_m;

  // Monitor methods
  Segment GetSegment();
  void UpdateCount(uint64_t num_checked);
  void Work();
};
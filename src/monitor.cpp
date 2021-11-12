#include <monitor.h>
#include <collatz.h>

#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <stdint.h>
#include <thread>
#include <mutex>

Segment Monitor::GetSegment() {
  std::lock_guard<std::mutex> lock(provider_mutex_m);
  if (done_m) {
    return Segment(-1, -1);
  }
  uint64_t size = std::min(checks_m - counter_control_m, segment_size_m);
  // Segment segment(std::max(counter_control_m, uint64_t(1)), counter_control_m + size);
  Segment segment(counter_control_m + 1, counter_control_m + size + 1);
  counter_control_m += size;
  done_m = counter_control_m >= checks_m;
  return segment;
}

void Monitor::UpdateCount(uint64_t num_checked) {
  std::lock_guard<std::mutex> lock(updater_mutex_m);
  checked_m += num_checked;

  auto ts = std::chrono::high_resolution_clock::now();
  uint64_t seconds = std::chrono::duration_cast<std::chrono::seconds>(ts - start_time_m).count();
  auto iter_p_sec = (checked_m / std::max(uint64_t(1), seconds));

  int h = seconds / 3600;
  int m = (seconds - (h * 3600)) / 60;
  int s = (seconds - ((m * 60) + (h * 3600)));

  std::cout 
    << "Numbers checked: " << checked_m / 1'000'000 
    << "M - Checks/sec: " << iter_p_sec / 1000 << "K - " 
    << h << "h" << m << "m" << s << "s\r" << std::flush; 
} 

void Monitor::Work() {
  while (1) {
    Segment s = GetSegment();
    if (s.start == -1) {
      break;
    }

    for (uint64_t num = s.start; num < s.end; num++) {
      Explore(num);
    }
    UpdateCount(s.end - s.start);
  }
}

void Monitor::Run() {
  std::vector<std::thread> threads(num_threads_m);
  start_time_m = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < num_threads_m; i++) {
    threads[i] = std::thread(&Monitor::Work, this);
  }
  for (auto &t : threads) {
    if (t.joinable()) {
      t.join();
    }
  }

  auto ts = std::chrono::high_resolution_clock::now();
  uint64_t seconds = std::chrono::duration_cast<std::chrono::seconds>(ts - start_time_m).count();
  auto iter_p_sec = (checked_m / std::max(uint64_t(1), seconds));
  int h = seconds / 3600;
  int m = (seconds - (h * 3600)) / 60;
  int s = (seconds - ((m * 60) + (h * 3600)));

  std::cout 
    << "Numbers checked: " << checked_m / 1'000'000 
    << "M - Checks/sec: " << iter_p_sec / 1000 << "K - " 
    << h << "h" << m << "m" << s << "s" << std::endl;

  std::cout << "-----------" << "\033[1m" << " RESULTS " << "\033[0m" << "-----------" << std::endl; 
  std::cout << "Numbers checked  : " << checked_m << std::endl;
  std::cout << "Time elapsed     : " << h << "h" << m << "m" << s << "s" << std::endl;
  std::cout << "-------------------------------" << std::endl;
}
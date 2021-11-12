#include <monitor.h>

#include <iostream>
#include <thread>

int main(int argc, char *argv[]) {
  int num_threads = std::thread::hardware_concurrency();
  std::cout << "num_threads: " << "\33[1m" << num_threads << "\33[0m" << std::endl;

  uint64_t checks = 10'000'000'000;
  uint64_t segment_size = 1'000'000;

  Monitor m(num_threads, checks, segment_size);
  m.Run();
}

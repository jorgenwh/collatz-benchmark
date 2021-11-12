#include <collatz.h>

uint64_t &Step(uint64_t &num) {
  if (num % 2 == 0) {
    num /= 2;
  } else {
    num = (num * 3) + 1;
  }
  return num;
}

void Explore(uint64_t num) {
  while (num != 1) {
    num = Step(num);
  }
}
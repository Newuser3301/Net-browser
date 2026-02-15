#include "src/platform/c_shim/include/net_platform.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

unsigned long long nb_now_ms(void) {
  // GetTickCount64: milliseconds since boot
  return (unsigned long long)GetTickCount64();
}

void nb_sleep_ms(unsigned int ms) {
  Sleep(ms);
}
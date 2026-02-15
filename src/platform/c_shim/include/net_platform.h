#pragma once
#ifdef __cplusplus
extern "C" {
#endif

// Minimal platform helpers (C ABI)
unsigned long long nb_now_ms(void);
void nb_sleep_ms(unsigned int ms);

#ifdef __cplusplus
}
#endif
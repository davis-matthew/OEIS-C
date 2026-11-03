/* omega_u64_to_u32: the number of distinct prime factors of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_OMEGA_U64_TO_U32_H
#define OEISC_UTILS_OMEGA_U64_TO_U32_H
#include <stdbool.h>
#include <stdint.h>

/* Count distinct prime factors of a uint64_t */
static unsigned int omega_u64_to_u32(uint64_t n) {
    unsigned int cnt = 0;
    if (n <= 1) return 0;
    if (n % 2ULL == 0) { cnt++; while (n % 2ULL == 0) n /= 2ULL; }
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) { cnt++; while (n % p == 0) n /= p; }
    }
    if (n > 1) cnt++;            /* remaining prime factor */
    return cnt;
}

#endif

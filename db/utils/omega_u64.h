/* omega_u64: the number of distinct prime factors of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_OMEGA_U64_H
#define OEISC_UTILS_OMEGA_U64_H
#include <stdint.h>

/* Helper to count distinct prime factors of a 64‑bit integer.  */
static seq_t omega_u64(uint64_t m)
{
    int cnt = 0;
    if (m % 2ULL == 0) {
        cnt++;
        while (m % 2ULL == 0) m /= 2ULL;
    }
    for (uint64_t p = 3ULL; p * p <= m; p += 2ULL) {
        if (m % p == 0) {
            cnt++;
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1ULL) cnt++;
    return (seq_t)cnt;
}

#endif

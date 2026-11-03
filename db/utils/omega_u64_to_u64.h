/* omega_u64_to_u64: the number of distinct prime factors of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_OMEGA_U64_TO_U64_H
#define OEISC_UTILS_OMEGA_U64_TO_U64_H
#include <stddef.h>
#include <stdint.h>

/* count distinct prime factors of x (omega function) */
static size_t omega_u64_to_u64(uint64_t x)
{
    size_t cnt = 0;
    if (x < 2)
        return 0;
    if ((x & 1ULL) == 0) {
        ++cnt;
        do { x >>= 1; } while ((x & 1ULL) == 0);
    }
    for (uint64_t p = 3; p * p <= x; p += 2) {
        if (x % p == 0) {
            ++cnt;
            do { x /= p; } while (x % p == 0);
        }
    }
    if (x > 1)
        ++cnt;
    return cnt;
}

#endif

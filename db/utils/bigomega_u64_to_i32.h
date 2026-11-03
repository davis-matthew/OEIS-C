/* bigomega_u64_to_i32: the number of prime factors of n, with multiplicity.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_BIGOMEGA_U64_TO_I32_H
#define OEISC_UTILS_BIGOMEGA_U64_TO_I32_H
#include <math.h>
#include <stdbool.h>
#include <stdint.h>

static int bigomega_u64_to_i32(uint64_t n)
{
    int cnt = 0;
    while ((n & 1ULL) == 0) {
        ++cnt;
        n >>= 1;
    }
    for (uint64_t p = 3; p * p <= n; p += 2) {
        while (n % p == 0) {
            ++cnt;
            n /= p;
        }
    }
    if (n > 1) ++cnt;
    return cnt;
}

#endif

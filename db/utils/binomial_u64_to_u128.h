/* binomial_u64_to_u128: the binomial coefficient C(n, k).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_BINOMIAL_U64_TO_U128_H
#define OEISC_UTILS_BINOMIAL_U64_TO_U128_H
#include <stdint.h>

static unsigned __int128 binomial_u64_to_u128(uint64_t m, uint64_t k) {
    unsigned __int128 c = 1;
    uint64_t i;
    if (k > m) return 0;
    for (i = 1; i <= k; i++)
        c = c * (unsigned __int128)(m - k + i) / i;
    return c;
}

#endif

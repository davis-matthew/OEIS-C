/* binomial_idx_to_i128: the binomial coefficient C(n, k).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_BINOMIAL_IDX_TO_I128_H
#define OEISC_UTILS_BINOMIAL_IDX_TO_I128_H
#include <stddef.h>
#include <stdint.h>

static inline __int128 binomial_idx_to_i128(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    unsigned __int128 r = 1;
    for (idx_t i = 1; i <= k; ++i) {
        r = r * (n - k + i);
        r /= i;
    }
    return (__int128)r;
}

#endif

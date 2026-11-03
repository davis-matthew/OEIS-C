/* binomial_idx: the binomial coefficient C(n, k).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_BINOMIAL_IDX_H
#define OEISC_UTILS_BINOMIAL_IDX_H
#include <stdint.h>

static inline idx_t binomial_idx(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    idx_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        __int128 tmp = (__int128)res * (n - k + i);
        res = (idx_t)(tmp / i);
    }
    return res;
}

#endif

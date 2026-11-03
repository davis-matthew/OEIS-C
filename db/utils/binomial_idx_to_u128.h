/* binomial_idx_to_u128: the binomial coefficient C(n, k).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_BINOMIAL_IDX_TO_U128_H
#define OEISC_UTILS_BINOMIAL_IDX_TO_U128_H
#include <stdint.h>

static inline unsigned __int128 binomial_idx_to_u128(idx_t N, idx_t K)
{
    if (K > N) return 0;
    if (K > N - K) K = N - K;
    unsigned __int128 result = 1;
    for (idx_t i = 1; i <= K; ++i) {
        result = result * (unsigned __int128)(N - K + i) / (unsigned __int128)i;
    }
    return result;
}

#endif

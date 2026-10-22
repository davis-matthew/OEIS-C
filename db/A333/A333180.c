/*
 * A333180  G.f.: Sum_{k>=1} (k * x^(k^2) * Product_{j=1..k} (1 + x^j)).
 * https://oeis.org/A333180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k≥1} k * f_k(n−k²) where f_k(m) is the number of
 *            subsets of {1,…,k} that sum to m
 * Range:     u8 n <= 109, u16 n <= 374, u32 n <= 1380, u64 n <= 5293,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static inline idx_t floor_sqrt(idx_t n)
{
    idx_t lo = 0, hi = n + 1;
    while (lo + 1 < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (mid > 0 && mid > n / mid)
            hi = mid;
        else
            lo = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t *dp = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!dp) abort();           /* memory allocation failed */
    dp[0] = 1;

    seq_t result = 0;
    idx_t max_k = floor_sqrt(n);

    for (idx_t k = 1; k <= max_k; ++k) {
        /* update dp in place: add element k to all subset sums */
        for (idx_t s = n; s >= k; --s)
            dp[s] += dp[s - k];

        idx_t diff = n - k * k;
        result += dp[diff] * (seq_t)k;
    }

    free(dp);
    return result;
}

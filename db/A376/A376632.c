/*
 * A376632  G.f.: Sum_{k>=0} x^(k^2) * Product_{j=1..k} (1 + x^(2*j)).
 * https://oeis.org/A376632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in Sum_{k>=0} x^(k^2) *
 *            Prod_{j=1..k}(1 + x^(2*j))
 * Range:     u8 n <= 258, u16 n <= 713, u32 n <= 2214, u64 n <= 7534,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    /* Return cached value if already computed */
    if (n < cache_cap)
        return cache[n];

    /* Allocate or enlarge cache to include n */
    if (cache_cap < n + 1) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
    }

    /* Initialise cache entries to zero */
    for (idx_t i = 0; i <= n; ++i)
        cache[i] = 0;

    /* DP array for subset sums of {1..k} */
    idx_t max_target = n / 2;
    seq_t *dp = (seq_t *)calloc(max_target + 1, sizeof(seq_t));
    if (!dp) exit(EXIT_FAILURE);
    dp[0] = 1;          /* only empty subset has sum 0 */

    /* k = 0 contributes to n = 0 */
    cache[0] += dp[0];

    /* Compute floor(sqrt(n)) without external helpers */
    idx_t limit_k = 0;
    while ((limit_k + 1) * (limit_k + 1) <= n)
        ++limit_k;

    /* Incrementally build DP for each k and add contributions */
    for (idx_t k = 1; k <= limit_k; ++k) {
        /* Update dp array if k is within the target range */
        if (k <= max_target) {
            for (idx_t s = max_target; ; --s) {
                dp[s] += dp[s - k];
                if (s == k) break;   /* avoid underflow when s becomes 0 */
            }
        }

        /* Add contributions to cache[n] for all possible sums t */
        for (idx_t t = 0; t <= max_target; ++t) {
            if (dp[t] != 0) {
                idx_t n_val = k * k + (idx_t)2 * t;
                if (n_val <= n)
                    cache[n_val] += dp[t];
            }
        }
    }

    free(dp);
    cache_cap = n + 1;
    return cache[n];
}

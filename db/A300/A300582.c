/*
 * A300582  Expansion of Product_{k>=1} 1 / (1 - 3*2^k*x^k).
 * https://oeis.org/A300582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k>=1} 1/(1 - 3*2^k x^k)
 *            Equivalent recurrence: a(0)=1; for n≥1 a(n) = Σ_{k=1}^{n}
 *            (3*2^k) * a(n-k) with unlimited repetitions of each k (unbounded
 *            coin change with weight 3*2^k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow2.h"

static seq_t *cached_dp = NULL;   /* dynamic array holding computed values */
static size_t cached_size = 0;    /* allocated size of cached_dp */

seq_t a(idx_t n)
{
    /* Expand the cache if needed */
    if (cached_size < n + 1) {
        /* free previous cache */
        free(cached_dp);
        /* allocate new cache with zero initialization */
        cached_dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
        if (!cached_dp) /* allocation failed */
            return 0;
        cached_size = (size_t)(n + 1);

        /* Unbounded coin‑change dynamic program */
        cached_dp[0] = 1;                 /* a(0) = 1 */
        for (idx_t k = 1; k <= n; ++k) {
            seq_t w = pow2(k);   /* 2^k */
            w = w * (seq_t)3;    /* 3*2^k */
            for (idx_t s = k; s <= n; ++s) {
                cached_dp[s] += cached_dp[s - k] * w;
            }
        }
    }
    return cached_dp[n];
}

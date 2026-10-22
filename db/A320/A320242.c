/*
 * A320242  Expansion of Product_{k=1..10} theta_3(q^k), where theta_3() is
 *          the Jacobi theta function.
 * https://oeis.org/A320242
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions to a1² + 2·a2² + … + 10·a10²
 *            = n (coefficients of the quadratic form are 1,2,…,10)
 * Range:     u8 n <= 14, u16 n <= 56, u32 n <= 888, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isqrt_idx_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses convolution of the generating
   functions θ₃(q^k) = Σ_{m∈ℤ} q^{k·m²} for k = 1..10.
   An incremental cache is maintained so repeated calls are
   efficient. */

#include <stdlib.h>
#include <string.h>
#include "../utils/isqrt_idx_u64.h"

static seq_t *cache = NULL;        /* cached results up to cache_len‑1 */
static idx_t  cache_len = 0;       /* number of cached elements */

/* Compute a(n). */
seq_t a(idx_t n)
{
    /* Extend cache if necessary. */
    if (n >= cache_len) {
        /* Recompute from scratch up to n. */
        idx_t N = n;                    /* target maximum */
        seq_t *curr = (seq_t*)calloc((size_t)(N + 1), sizeof(seq_t));
        seq_t *next = (seq_t*)calloc((size_t)(N + 1), sizeof(seq_t));
        if (!curr || !next) { /* memory allocation failed */
            free(curr); free(next);
            return 0; /* return zero (should not happen in normal use) */
        }
        curr[0] = 1;                     /* contribution of all zero coordinates */

        for (int k = 1; k <= 10; ++k) {
            /* zero 'next' array */
            memset(next, 0, (size_t)(N + 1) * sizeof(seq_t));

            for (idx_t i = 0; i <= N; ++i) {
                seq_t val = curr[i];
                if (!val) continue;
                /* maximum m with k*m² <= N-i */
                idx_t max_m = isqrt_idx_u64((N - i) / (idx_t)k);
                for (idx_t m = 0; m <= max_m; ++m) {
                    idx_t delta = k * (m * m);
                    idx_t j = i + delta;
                    seq_t add = val;
                    if (m > 0) add = add + add; /* multiply by 2 for ±m */
                    next[j] += add;
                }
            }
            /* Swap buffers for next iteration */
            seq_t *tmp = curr;
            curr = next;
            next = tmp;
        }
        /* Replace global cache with computed values */
        free(cache);
        cache = curr;                     /* curr now holds final results */
        free(next);                       /* 'next' is the old buffer, free it */
        cache_len = N + 1;
    }
    return cache[n];
}

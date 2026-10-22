/*
 * A364450  Number of partitions of [n] without prime sized blocks.
 * https://oeis.org/A364450
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k ≤ n, k not prime} C(n‑1, k‑1) * a(n‑k)
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 18, u64 n <= 29, u128 n <= 47,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial value: a(0) = 1, where C(m, r) is the binomial coefficient. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t *dp = NULL;   /* memoisation array */
static idx_t dp_cap = 0;   /* largest index stored (dp_cap ≥ 0) */

/* Return the n‑th term of the sequence for arbitrary‑precision or
   Fixed‑width `seq_t`.  The function is safe for any n ≥ 0 and can
   be called in arbitrary order. */
seq_t a(idx_t n)
{
    /* Initialise cache on first call.  dp[0] = 1. */
    if (dp == NULL) {
        dp = (seq_t *)malloc(sizeof(seq_t));
        if (!dp) return 0;            /* allocation failed */
        dp[0] = 1;
        dp_cap = 0;
    }

    /* If already computed, return cached value. */
    if (n <= dp_cap) return dp[n];

    /* Resize the cache to accommodate indices up to n. */
    idx_t old_cap = dp_cap;
    dp_cap = n;
    seq_t *new_dp = (seq_t *)realloc(dp, (dp_cap + 1) * sizeof(seq_t));
    if (!new_dp) {
        /* On OOM, keep the array unchanged and return 0. */
        return 0;
    }
    dp = new_dp;

    /* Compute terms for all m in (old_cap, n]. */
    for (idx_t m = old_cap + 1; m <= n; ++m) {
        seq_t sum = 0;
        seq_t cur = 1;   /* C(m-1, 0) */

        for (idx_t j = 0; j < m; ++j) {
            idx_t k = j + 1;               /* block size */

            if (!is_prime(k)) {            /* allowed block size */
                sum += cur * dp[m - k];
            }

            if (j < m - 1) {               /* advance binomial coefficient */
                cur = cur * (seq_t)(m - 1 - j);
                cur = cur / (seq_t)(j + 1);
            }
        }
        dp[m] = sum;
    }

    return dp[n];
}

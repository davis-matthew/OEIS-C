/*
 * A327710  Number of compositions of n into distinct parts such that the
 *          difference between any two parts is at least two.
 * https://oeis.org/A327710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k>=0} k! * p(n - k^2, k) where p(m, k) is the number
 *            of partitions of m into at most k parts
 * Range:     u8 n <= 20, u16 n <= 46, u32 n <= 114, u64 n <= 304,
 *            u128 n <= 854, big any n
 * Validated: standard, pass, 66 terms through n = 8782 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

static seq_t *cache_a = NULL;      /* array of a(n) values */
static size_t cache_cap = 0;       /* current capacity of cache_a */

/* Compute a(n) */
seq_t a(idx_t n)
{
    /* Extend cache if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_limit = (size_t)n;
        /* Compute Kmax = floor(sqrt(new_limit)) */
        size_t Kmax = (size_t)sqrt((double)new_limit);

        /* Allocate and zero-initialize temporary arrays */
        seq_t *dp = mem_new(seq_t, new_limit + 1);   /* partitions array */
        seq_t *factorial = mem_new(seq_t, Kmax + 1); /* factorial array */
        if (dp == NULL || factorial == NULL) {
            /* Allocation failure – return 0 as a safe fallback */
            mem_free(dp);
            mem_free(factorial);
            return (seq_t)0;
        }

        /* New cache for a(n) */
        seq_t *new_cache = mem_new(seq_t, new_limit + 1);
        if (new_cache == NULL) {
            mem_free(dp);
            mem_free(factorial);
            return (seq_t)0;
        }

        /* Base case k = 0 */
        dp[0] = 1;
        factorial[0] = 1;
        new_cache[0] = 1;            /* a(0) = 1 */

        /* Iterate over k = 1 .. Kmax */
        for (size_t k = 1; k <= Kmax; ++k) {
            /* Update partition DP with part size k */
            for (idx_t s = k; s <= (idx_t)new_limit; ++s) {
                dp[s] = dp[s] + dp[s - k];
            }

            /* Compute factorial of k */
            factorial[k] = factorial[k - 1] * (seq_t)k;

            /* Add contribution to a[n] for all n >= k^2 */
            idx_t k2 = (idx_t)(k * k);
            for (idx_t nval = k2; nval <= (idx_t)new_limit; ++nval) {
                new_cache[nval] = new_cache[nval] + factorial[k] * dp[nval - k2];
            }
        }

        /* Release old cache and replace */
        mem_free(cache_a);
        cache_a = new_cache;
        cache_cap = new_limit + 1;

        /* Free temporary arrays */
        mem_free(dp);
        mem_free(factorial);
    }

    /* Return cached value */
    return cache_a[(size_t)n];
}

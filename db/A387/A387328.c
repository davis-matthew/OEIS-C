/*
 * A387328  Number of integer partitions of n whose parts have choosable sets
 *          of integer partitions.
 * https://oeis.org/A387328
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A387328: number of integer partitions of n such that each part k
 * can appear at most p(k) times, where p(k) is the ordinary partition
 * number of k.  Equivalently, the generating function is
 *   ∏_{k≥1} (1 + x^k + x^{2k} + … + x^{p(k)k})
 * This algorithm computes the coefficients by dynamic programming.
 * For each part size k we use an unbounded knapsack update when
 * p(k) is large enough (so the bound is irrelevant), otherwise we
 * perform a bounded update with a sliding‑window technique.  The
 * routine keeps a static cache of the first (max_n+1) values so
 * that repeated calls are O(1) for previously seen n.  The implementation
 * is valid C11 and C++17; seq_t and idx_t are supplied by the build. */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Cache of previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* number of stored entries = max_n+1 */

    /* Grow the cache if we need to reach a larger n */
    if (n + (idx_t)1 > cache_cap) {
        idx_t new_cap = n + (idx_t)1;              /* compute up to n */

        /* allocate the new cache */
        seq_t *new_cache = (seq_t *)calloc((size_t)new_cap, sizeof(seq_t));
        if (!new_cache) return (seq_t)0;           /* out of memory */

        /* 1. Compute partition numbers p[0..new_cap-1] */
        seq_t *p = (seq_t *)calloc((size_t)new_cap, sizeof(seq_t));
        if (!p) {
            free(new_cache);
            return (seq_t)0;
        }
        p[0] = (seq_t)1;
        for (idx_t m = 1; m < new_cap; ++m) {
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; ; ++k) {
                idx_t pent1 = k * (3 * k - 1) / 2;
                if (pent1 > m) break;
                if (k & 1) sum += p[m - pent1];
                else sum -= p[m - pent1];
                idx_t pent2 = k * (3 * k + 1) / 2;
                if (pent2 > m) continue;
                if (k & 1) sum += p[m - pent2];
                else sum -= p[m - pent2];
            }
            p[m] = sum;
        }

        /* 2. DP for the bounded-partition sequence */
        seq_t *dp = (seq_t *)calloc((size_t)new_cap, sizeof(seq_t));
        if (!dp) {
            free(p);
            free(new_cache);
            return (seq_t)0;
        }
        dp[0] = (seq_t)1; /* only one way to partition 0 */

        for (idx_t k = 1; k < new_cap; ++k) {
            /* decide whether part k is effectively unbounded */
            if (p[k] > (seq_t)(new_cap - 1) / k) {           /* unbounded */
                for (idx_t s = k; s < new_cap; ++s)
                    dp[s] += dp[s - k];
            } else {                                      /* bounded */
                size_t cap = (size_t)p[k];                  /* cap <= new_cap */
                seq_t *next = (seq_t *)calloc((size_t)new_cap, sizeof(seq_t));
                if (!next) {
                    free(dp);
                    free(p);
                    free(new_cache);
                    return (seq_t)0;
                }
                for (idx_t r = 0; r < k; ++r) {
                    idx_t pos = r;
                    idx_t count = 0;
                    seq_t sum = (seq_t)0;
                    while (pos < new_cap) {
                        sum += dp[pos];
                        if (count > cap)
                            sum -= dp[pos - (cap + 1) * k];
                        next[pos] = sum;
                        ++count;
                        pos += k;
                    }
                }
                free(dp);
                dp = next;
            }
        }

        /* copy DP results into the new cache and free temp arrays */
        for (idx_t i = 0; i < new_cap; ++i)
            new_cache[i] = dp[i];
        free(dp);
        free(p);

        /* replace the old cache with the new one */
        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* return the requested term from the cache */
    return cache[n];
}

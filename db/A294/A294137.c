/*
 * A294137  Number of compositions (ordered partitions) of n into nontrivial
 *          divisors of n.
 * https://oeis.org/A294137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Number of compositions (ordered partitions) of n into nontrivial
 *            divisors of n
 * Range:     u8 n <= 23, u16 n <= 35, u32 n <= 59, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache for already computed values. */
    static seq_t *cache = NULL;   /* cache[0..cache_cap] */
    static idx_t cache_cap = 0;

    /* Temporary buffer for DP of a single n. */
    static seq_t *tmp = NULL;     /* tmp[0..tmp_cap] */
    static idx_t tmp_cap = 0;

    /* Ensure the base case a(0) = 1 is stored. */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* unlikely */
        cache[0] = (seq_t)1;
        cache_cap = 0;
    }

    /* Compute all values up to n that are not yet cached. */
    if (n > cache_cap) {
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache && n + 1 > 0) return (seq_t)0; /* allocation failure */
        cache = new_cache;

        /* For each new m, compute a(m) by DP over its nontrivial divisors. */
        for (idx_t m = cache_cap + 1; m <= n; ++m) {
            /* Resize temporary buffer if necessary. */
            if (m > tmp_cap) {
                seq_t *new_tmp = (seq_t *)realloc(tmp, (m + 1) * sizeof(seq_t));
                if (!new_tmp && m + 1 > 0) { cache[m] = (seq_t)0; continue; }
                tmp = new_tmp;
                tmp_cap = m;
            }

            /* Collect nontrivial divisors of m (excluding 1 and m). */
            idx_t divs[128];
            size_t dcnt = 0;
            for (idx_t d = 2; d * d <= m; ++d) {
                if (m % d == 0) {
                    divs[dcnt++] = d;
                    idx_t e = m / d;
                    if (e != d && e < m) {
                        divs[dcnt++] = e;
                    }
                }
            }

            /* DP: dp[0] = 1, dp[s] = sum_{d in divs, d<=s} dp[s-d]. */
            tmp[0] = (seq_t)1;
            for (idx_t s = 1; s <= m; ++s) {
                seq_t sum = 0;
                for (size_t i = 0; i < dcnt; ++i) {
                    idx_t d = divs[i];
                    if (d <= s) {
                        sum += tmp[s - d];
                    }
                }
                tmp[s] = sum;
            }
            cache[m] = tmp[m];
        }
        cache_cap = n;
    }

    return cache[n];
}

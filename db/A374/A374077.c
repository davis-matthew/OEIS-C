/*
 * A374077  Expansion of Product_{k>=1} 1 / (1 + x^(5*k-2)).
 * https://oeis.org/A374077
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥1} 1/(1 + x^{5k-2})
 * Range:     i8 n <= 75, i16 n <= 75, i32 n <= 75, i64 n <= 75, i128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: dynamic programming with sign; dp[i] = dp[i] – dp[i‑m]
   for each m = 5k−2 (k = 1,2,…) and for all i ≥ m.   */

#include <stdlib.h>

/* Cache of previously computed values. */
static seq_t *dp_cache = NULL;        /* array holding a(0)…a(n) */
static idx_t  dp_cache_size = 0;      /* number of entries stored (>=1) */

/* Recompute the array up to n (inclusive) and store it in dp_cache. */
static void recompute_until(idx_t n)
{
    size_t sz = (size_t)(n + 1);
    seq_t *newdp = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!newdp) exit(EXIT_FAILURE);      /* abort if allocation fails */

    /* initialize */
    for (size_t i = 0; i < sz; ++i) newdp[i] = (seq_t)0;
    newdp[0] = (seq_t)1;                 /* a(0) = 1 */

    /* process each factor 1/(1 + x^m) with m = 5k-2 */
    for (idx_t k = 1;; ++k) {
        idx_t m = 5 * k - 2;
        if (m > n) break;
        for (idx_t i = m; i <= n; ++i) {
            newdp[i] -= newdp[i - m];
        }
    }

    /* replace old cache */
    free(dp_cache);
    dp_cache = newdp;
    dp_cache_size = sz;
}

/* Ensure that dp_cache contains values up to at least n. */
static void ensure_idx(idx_t n)
{
    if (dp_cache == NULL || n >= dp_cache_size) {
        recompute_until(n);
    }
}

seq_t a(idx_t n)
{
    ensure_idx(n);
    return dp_cache[(size_t)n];
}

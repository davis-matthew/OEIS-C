/*
 * A394200  Expansion of Product_{k>=1} (1 + x^(k*(k+2))) / (1 - x^(k*(k+2))).
 * https://oeis.org/A394200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   computed by dynamic programming for the expansion of ∏_{k≥1} (1
 *            + x^{k(k+2)}) / (1 - x^{k(k+2)})
 * Range:     u8 n <= 76, u16 n <= 234, u32 n <= 878, u64 n <= 4033,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Forward declarations of helper functions. */
static void recompute(idx_t max_n);

/* Static cache for sequence values. */
static seq_t *seq_cache = NULL;   /* holds a(0)..a(max_n) */
static idx_t seq_max   = 0;        /* largest index cached           */

/* Public function: value at index n. */
seq_t a(idx_t n) {
    if (seq_cache == NULL || n > seq_max)
        recompute(n);
    return seq_cache[n];
}

/* Compute all terms up to max_n and store them in the cache. */
static void recompute(idx_t max_n) {
    size_t sz = (size_t)max_n + 1;
    seq_t *dp = (seq_t*)calloc(sz, sizeof(seq_t));
    if (!dp) return;  /* allocation failed; leave old cache untouched */

    dp[0] = (seq_t)1;  /* a(0) = 1 */

    for (idx_t k = 1; ; ++k) {
        idx_t m = k * (k + 2);   /* part size k(k+2) */
        if (m > max_n) break;

        seq_t *next = (seq_t*)calloc(sz, sizeof(seq_t));
        if (!next) { free(dp); return; }

        /* For residues modulo m update the DP. */
        for (idx_t r = 0; r < m; ++r) {
            seq_t sum = (seq_t)0;
            for (idx_t i = r; i <= max_n; i += m) {
                seq_t val = dp[i];
                seq_t add = sum * (seq_t)2;
                next[i] = val + add;
                sum += val;
            }
        }

        free(dp);
        dp = next;
    }

    if (seq_cache) free(seq_cache);
    seq_cache = dp;
    seq_max   = max_n;
}

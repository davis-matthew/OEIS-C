/*
 * A301706  Number of rooted thrice-partitions of n.
 * https://oeis.org/A301706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A301706: Number of rooted thrice-partitions of n
   Computed by dynamic programming using the combinatorial
   definition.  The value a(n) is derived from weighted partitions
   where parts of size k are weighted by the number of rooted
   twice-partitions of k, which in turn are weighted by the number
   of rooted partitions of k.  The algorithm keeps the values
   in static arrays and extends them only when a larger n is
   requested.  Complexity is O(n²) for the largest n requested,
   which is fast enough for n up to about 10000.  */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          // undefined for n = 0
    static idx_t cur_max = 0;
    static seq_t *h = NULL; /* h[k] = number of rooted partitions of k (partition of k-1) */
    static seq_t *g = NULL; /* g[k] = number of rooted twice‑partitions of k */
    static seq_t *f = NULL; /* f[k] = a(k), the desired sequence */

    /* Extend precomputation when a larger n is requested */
    if (n > cur_max) {
        idx_t new_max = n;

        /* Allocate new arrays */
        seq_t *new_h = (seq_t*)malloc((new_max + 1) * sizeof(seq_t));
        seq_t *new_g = (seq_t*)malloc((new_max + 1) * sizeof(seq_t));
        seq_t *new_f = (seq_t*)malloc((new_max + 1) * sizeof(seq_t));

        /* 1. Partition numbers p[0..new_max-1] (number of partitions of m) */
        seq_t *p = (seq_t*)calloc(new_max, sizeof(seq_t));
        p[0] = 1;
        for (idx_t i = 1; i < new_max; ++i) {
            for (idx_t j = i; j < new_max; ++j) {
                p[j] += p[j - i];
            }
        }

        /* 2. h[k] = partition of k-1 */
        new_h[0] = 0;                       /* unused slot */
        for (idx_t k = 1; k <= new_max; ++k) {
            new_h[k] = p[k - 1];
        }

        /* 3. DP arrays for weighted partitions */
        idx_t max_sum = new_max - 1;        /* largest sum that can occur */
        seq_t *dp  = (seq_t*)calloc(max_sum + 1, sizeof(seq_t)); /* for g */
        seq_t *dp2 = (seq_t*)calloc(max_sum + 1, sizeof(seq_t)); /* for f */
        dp[0]  = 1;
        dp2[0] = 1;

        /* 4. Process each part size k = 1 .. new_max-1 */
        for (idx_t k = 1; k < new_max; ++k) {
            /* Weight for rooted twice-partitions: g(k) = dp[k-1] (after previous steps) */
            seq_t weight_g = (k == 1) ? (seq_t)1 : dp[k - 1];
            for (idx_t s = k; s <= max_sum; ++s) {
                dp2[s] += weight_g * dp2[s - k];
            }

            /* Weight for rooted partitions: h(k) = partition(k-1) */
            seq_t weight_h = new_h[k];
            for (idx_t s = k; s <= max_sum; ++s) {
                dp[s] += weight_h * dp[s - k];
            }
        }

        /* 5. Extract g and f values from DP tables */
        for (idx_t idx = 1; idx <= new_max; ++idx) {
            new_g[idx] = dp[idx - 1];
            new_f[idx] = dp2[idx - 1];
        }

        /* 6. Free temporary and old arrays */
        free(p);
        free(dp);
        free(dp2);
        free(h);
        free(g);
        free(f);

        /* 7. Store new arrays in static variables */
        h = new_h;
        g = new_g;
        f = new_f;
        cur_max = new_max;
    }

    return f[n];
}

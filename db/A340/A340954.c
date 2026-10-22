/*
 * A340954  Number of ways to write n as an ordered sum of 9 nonzero
 *          triangular numbers.
 * https://oeis.org/A340954
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 51, u32 n <= 739, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A340954:
   a(n) = number of ways to write n as an ordered sum of 9 nonzero
   triangular numbers (T_k = k(k+1)/2, k ≥ 1). */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n < 9) return 0;          /* offset of the sequence */

    static seq_t *dp[10] = {0};   /* dp[parts][sum] */
    static idx_t max_cached = 0;  /* largest n for which dp is built */
    static idx_t *tri = NULL;     /* list of triangular numbers ≤ max_cached */
    static size_t tri_len = 0;    /* number of triangular numbers */

    /* Rebuild tables if we have seen a larger n */
    if (n > max_cached) {
        idx_t new_max = n;

        /* Compute triangular numbers up to new_max */
        size_t idx = 0;
        for (idx_t k = 1;; ++k) {
            idx_t t = k * (k + 1) / 2;
            if (t > new_max) break;
            idx++;   /* count first to know size */
        }
        tri_len = idx;
        idx_t *new_tri = (idx_t *)realloc(tri, tri_len * sizeof(idx_t));
        if (!new_tri) return 0;    /* allocation failure */
        tri = new_tri;

        /* Fill the triangular numbers */
        idx = 0;
        for (idx_t k = 1;; ++k) {
            idx_t t = k * (k + 1) / 2;
            if (t > new_max) break;
            tri[idx++] = t;
        }

        /* Allocate and clear DP arrays */
        for (int p = 0; p < 10; ++p) {
            if (dp[p]) free(dp[p]);
            dp[p] = (seq_t *)calloc(new_max + 1, sizeof(seq_t));
            if (!dp[p]) return 0;   /* allocation failure */
        }
        dp[0][0] = 1;                 /* base case */

        /* Dynamic programming: dp[parts][sum] */
        for (int p = 1; p <= 9; ++p) {
            for (idx_t s = 0; s <= new_max; ++s) {
                seq_t total = 0;
                for (size_t i = 0; i < tri_len; ++i) {
                    idx_t t = tri[i];
                    if (s < t) break;  /* triangles are increasing */
                    total += dp[p - 1][s - t];
                }
                dp[p][s] = total;
            }
        }

        max_cached = new_max;
    }

    /* Return the precomputed answer */
    return dp[9][n];
}

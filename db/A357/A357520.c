/*
 * A357520  Expansion of Product_{k>=0} (1 - x^Lucas(k)).
 * https://oeis.org/A357520
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 93, i16 n <= 93, i32 n <= 93, i64 n <= 93, i128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A357520: Expansion of Product_{k>=0} (1 - x^{Lucas(k)}) */
/* The coefficient a(n) is the alternating‑sum over all subsets of Lucas numbers summing to n.
 * Computed by iterating over each Lucas number L ≤ n, updating a DP array of length n+1:
 *     dp[0] = 1;
 *     for each L: for i = n down to L: dp[i] -= dp[i-L];
 * The result is dp[n]. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate and zero‑initialize the coefficient array */
    seq_t *dp = (seq_t*)calloc((size_t)n + 1, sizeof(seq_t));
    if (!dp) return 0;   /* allocation failure */
    dp[0] = 1;           /* base case: empty subset */

    /* Handle initial Lucas numbers L(0)=2 and L(1)=1 */
    if (n >= 2) {
        idx_t L = 2;
        for (idx_t i = n; ; --i) {
            dp[i] -= dp[i - L];
            if (i == L) break;
        }
    }
    if (n >= 1) {
        idx_t L = 1;
        for (idx_t i = n; ; --i) {
            dp[i] -= dp[i - L];
            if (i == L) break;
        }
    }

    /* Generate subsequent Lucas numbers L(k)=L(k-1)+L(k-2) and process them */
    idx_t prev2 = 2; /* L(0) */
    idx_t prev1 = 1; /* L(1) */
    while (1) {
        idx_t L = prev2 + prev1;   /* next Lucas number */
        if (L > n) break;
        for (idx_t i = n; ; --i) {
            dp[i] -= dp[i - L];
            if (i == L) break;
        }
        prev2 = prev1;
        prev1 = L;
    }

    seq_t res = dp[n];
    free(dp);
    return res;
}

/*
 * A355358  Coefficients in the expansion of A(x) = 1 / Product_{n>=0} (1 -
 *          x^(13*n+1))*(1 - x^(13*n+3))*(1 - x^(13*n+4))*(1 - x^(13*n+9))*(1
 *          - x^(13*n+10))*(1 - x^(13*n+12)).
 * https://oeis.org/A355358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 82, u32 n <= 253, u64 n <= 850,
 *            u128 n <= 3042, big any n
 * Helpers:   is_residue
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A355358
    a(n) = number of partitions of n into parts congruent to
    1, 3, 4, 9, 10, 12 (mod 13).  G.f.: 1 / Π_{m≡1,3,4,9,10,12 (13)} (1‑x^m)
*/
#include <stdlib.h>
#include "../utils/is_residue.h"

/* memoised DP – recomputed only when a larger index is requested */
seq_t a(idx_t n)
{
    /* static data structures for incremental computation */
    static seq_t *dp = NULL;     /* array of coefficients, indexed 0..max_n */
    static idx_t max_n = 0;      /* last index for which dp is valid */

    /* initialise if first call */
    if (dp == NULL) {
        dp = (seq_t *)calloc(1, sizeof(seq_t));
        if (!dp) return (seq_t)0;   /* allocation failure */
        dp[0] = (seq_t)1;
        max_n = 0;
    }

    /* if we already have a value for this index, return it */
    if (n <= max_n) return dp[n];

    /* allocate a larger array */
    idx_t new_n = n;
    size_t sz = (size_t)(new_n + 1);
    seq_t *new_dp = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!new_dp) return (seq_t)0;   /* allocation failure */
    new_dp[0] = (seq_t)1;           /* base case */

    /* classic coin‑change DP: for each admissible part m
       add its contribution to all sums >= m */
    for (idx_t m = 1; m <= new_n; ++m) {
        if (!is_residue(m)) continue;
        for (idx_t j = m; j <= new_n; ++j) {
            new_dp[j] += new_dp[j - m];
        }
    }

    /* replace old array and update the maximum index */
    free(dp);
    dp = new_dp;
    max_n = new_n;

    return dp[n];
}

/*
 * A275312  Number of set partitions of [n] with increasing block sizes.
 * https://oeis.org/A275312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of set partitions of [n] with increasing block
 *            sizes The implementation uses dynamic programming on remaining
 *            elements and the minimal allowed block size
 * Range:     u8 n <= 9, u16 n <= 13, u32 n <= 21, u64 n <= 34, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static storage for memoised DP rows */
    static seq_t **dp = NULL;          /* rows of DP values */
    static idx_t  dp_len = 0;          /* number of rows currently stored */

    /* initialise DP for n == 0 */
    if (dp_len == 0) {
        dp = (seq_t **)malloc(sizeof(seq_t*));
        dp[0] = (seq_t *)malloc(sizeof(seq_t) * 1);
        dp[0][0] = (seq_t)1;          /* a(0) = 1 */
        dp_len = 1;
    }

    /* extend DP up to requested n */
    if (n >= dp_len) {
        /* reallocate DP table to hold new rows */
        dp = (seq_t **)realloc(dp, sizeof(seq_t*) * (n + 1));
        for (idx_t rem = dp_len; rem <= n; ++rem)
        {
            /* allocate current row */
            seq_t *row = (seq_t *)malloc(sizeof(seq_t) * (rem + 1));
            dp[rem] = row;

            /* temporary array for binomial coefficients C(rem-1, k-1) */
            seq_t *binom = (seq_t *)malloc(sizeof(seq_t) * (rem + 1));
            if (rem >= 1) {
                binom[1] = (seq_t)1;           /* C(rem-1,0) = 1 */
                for (idx_t k = 2; k <= rem; ++k) {
                    binom[k] = binom[k-1] *
                               (seq_t)(rem - k + 1) /
                               (seq_t)(k - 1);
                }
            }

            /* compute suffix sums of U[k] = binom[k] * dp[rem-k][k] */
            seq_t suffix = (seq_t)0;
            for (idx_t k = rem; k > 0; --k) {
                idx_t remk = rem - k;
                seq_t val;
                if (remk == 0) {
                    val = (seq_t)1;                          /* empty partition */
                } else if (k <= remk) {
                    val = dp[remk][k];                      /* use earlier computed DP */
                } else {
                    val = (seq_t)0;                          /* impossible */
                }
                seq_t u = binom[k] * val;
                suffix = u + suffix;
                row[k-1] = suffix;                           /* dp[rem][last] = suffix[last+1] */
            }
            row[rem] = (seq_t)0;                               /* dp[rem][rem] = 0 */

            free(binom);
        }
        dp_len = n + 1;
    }

    return dp[n][0];         /* a(n) is dp[n][0] */
}

/*
 * A347661  Number of partitions of n into at most 6 powers of 2.
 * https://oeis.org/A347661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into at most 6 powers of 2
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#define MAX_PARTS 6

seq_t a(idx_t n)
{
    /* Generate all powers of 2 not exceeding n */
    idx_t powers[64];
    size_t np = 0;
    for (idx_t p = 1; p <= n; p <<= 1) {
        powers[np++] = p;
    }

    /* DP table: (n+1) rows, MAX_PARTS+1 columns (k=0..6) */
    size_t rows     = (size_t)(n + 1);
    size_t cols     = MAX_PARTS + 1;          /* 7 columns */
    size_t cells    = rows * cols;
    seq_t *dp = (seq_t *)malloc(cells * sizeof(seq_t));
    if (!dp) return (seq_t)0;                 /* out of memory */

    /* initialise to zero */
    for (size_t i = 0; i < cells; ++i) dp[i] = 0;
    dp[0] = 1;                                 /* dp[0][0] = 1 */

    /* Unbounded knapsack over the powers of 2 */
    for (size_t pi = 0; pi < np; ++pi) {
        idx_t val = powers[pi];
        for (idx_t s = val; s <= n; ++s) {
            seq_t *dp_s     = dp + (size_t)s * cols;
            const seq_t *dp_dv = dp + (size_t)(s - val) * cols;
            for (size_t k = 1; k <= MAX_PARTS; ++k) {
                dp_s[k] = dp_s[k] + dp_dv[k - 1];
            }
        }
    }

    /* Sum partitions with up to 6 parts */
    seq_t result = 0;
    seq_t *dp_n  = dp + (size_t)n * cols;
    for (size_t k = 0; k <= MAX_PARTS; ++k) {
        result = result + dp_n[k];
    }

    free(dp);
    return result;
}

/*
 * A347785  Number of compositions (ordered partitions) of n into at most 4
 *          powers of 2.
 * https://oeis.org/A347785
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of compositions of n into at most 4 powers of 2
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* External linkage function as required */
seq_t a(idx_t n)
{
    /* Static state to memoize values across calls */
    static seq_t *dp = NULL;          /* storage of DP values: dp[n*5 + k] = f[n][k] */
    static size_t dp_capacity = 0;    /* total allocated cells (n+1)*5 */
    static idx_t max_n = 0;           /* largest n for which DP has been computed */

    /* Ensure DP is calculated up to n */
    if (dp == NULL) {
        /* Initial allocation for n = 0 */
        dp_capacity = 5;                     /* (0+1)*5 */
        dp = (seq_t*)malloc(dp_capacity * sizeof(seq_t));
        if (!dp) { /* allocation failed – abort gracefully */ abort(); }

        /* dp[0][0] = 1, others 0 */
        for (int k = 0; k < 5; ++k) dp[k] = (k == 0) ? 1 : 0;
        max_n = 0;
    }

    /* Grow storage if necessary */
    size_t needed = ((size_t)n + 1) * 5;
    if (needed > dp_capacity) {
        dp_capacity = needed;
        seq_t *new_dp = (seq_t*)realloc(dp, dp_capacity * sizeof(seq_t));
        if (!new_dp) { /* reallocation failed – abort */ abort(); }
        dp = new_dp;
    }

    /* Compute DP rows from max_n+1 up to n */
    for (idx_t m = max_n + 1; m <= n; ++m) {
        size_t base = (size_t)m * 5;
        /* Initialise current row to zero (except dp[m][0] stays 0) */
        for (int k = 0; k < 5; ++k) dp[base + k] = 0;
        /* Sum over all powers of two <= m */
        idx_t p = 1;
        while (p <= m) {
            size_t prev = (size_t)(m - p) * 5;
            for (int k = 1; k <= 4; ++k) {
                dp[base + k] += dp[prev + (k - 1)];
            }
            p <<= 1;
        }
    }
    max_n = n;

    /* Return a(n) = sum_{k=0..4} f[n][k] */
    seq_t result = 0;
    size_t base = (size_t)n * 5;
    for (int k = 0; k <= 4; ++k) result += dp[base + k];
    return result;
}

/*
 * A308873  Sum of the largest parts in the partitions of n into 6 parts.
 * https://oeis.org/A308873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{partitions of n into 6 positive parts} (largest
 *            part)
 * Range:     u8 n <= 16, u16 n <= 45, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation by dynamic programming over the largest part. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* n < 6 -> no 6‑part partitions */
    if (n < 6)
        return 0;

    /* dp[i][j] : number of partitions of j into exactly i parts,
       each part ≤ current largest part.   i = 0..5, j = 0..n. */
    const idx_t max_sum = n;
    const size_t dim = (size_t)max_sum + 1;   /* length of each row */
    const size_t rows = 6;                    /* i = 0..5 */

    /* allocate and zero‑initialize the DP table */
#ifdef __cplusplus
    seq_t *dp = new seq_t[rows * dim]();      /* constructors called → value 0 */
#else
    seq_t *dp = (seq_t *)calloc(rows * dim, sizeof(seq_t));
#endif
    if (!dp)
        return 0;                              /* allocation failure (unlikely) */

    /* one way to partition 0 into 0 parts */
    dp[0] = 1;

    seq_t result = 0;

    /* iterate over possible largest part values a */
    for (idx_t a = 1; a <= n - 5; ++a) {
        /* update DP with the new part value a for i = 1..5 */
        for (size_t i = 1; i <= 5; ++i) {
            size_t base_i        = i * dim;
            size_t base_i_minus  = (i - 1) * dim;
            for (idx_t j = a; j <= max_sum; ++j) {
                dp[base_i + j] += dp[base_i_minus + (j - a)];
            }
        }

        /* dp[5][n-a] gives number of partitions of n with largest part exactly a */
        idx_t rem = n - a;
        seq_t cnt = dp[5 * dim + rem];
        result += (seq_t)a * cnt;
    }

    /* free DP table */
#ifdef __cplusplus
    delete[] dp;
#else
    free(dp);
#endif

    return result;
}

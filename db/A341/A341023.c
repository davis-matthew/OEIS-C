/*
 * A341023  Number of partitions of n into 6 distinct nonzero triangular
 *          numbers.
 * https://oeis.org/A341023
 *
 * Offset:    56    (a(56) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into 6 distinct nonzero
 *            triangular numbers
 * Range:     u8 n <= 156, u16 n <= 156, u32 n <= 156, u64 n <= 156,
 *            u128 n <= 156, big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    /* sequence is defined for n >= 56; behavior below is unspecified */
    if (n < 56) return (seq_t)0;

    /* collect all triangular numbers <= n */
    idx_t tri[200];
    size_t tcnt = 0;
    for (idx_t k = 1;; ++k) {
        idx_t t = k * (k + 1) / 2;
        if (t > n) break;
        tri[tcnt++] = t;
    }

    const size_t rows = 7;                       /* 0..6 distinct triangular numbers */
    const size_t cols = (size_t)n + 1;           /* sums 0..n */

    /* allocate DP table: dp[j][s] = number of ways to achieve sum s with j distinct terms */
    seq_t *dp = seq_alloc_new_array(rows * cols);
    for (size_t i = 0; i < rows * cols; ++i) dp[i] = (seq_t)0; /* ensure zeroed */
    dp[0 * cols + 0] = (seq_t)1;                 /* one way to form 0 with 0 terms */

    /* DP transition: iterate over triangular numbers */
    for (size_t idx = 0; idx < tcnt; ++idx) {
        idx_t t = tri[idx];
        for (int j = 6; j >= 1; --j) {
            size_t base_cur  = (size_t)j * cols;
            size_t base_prev = (size_t)(j - 1) * cols;
            for (idx_t s = t; s <= n; ++s) {
                dp[base_cur + s] += dp[base_prev + (s - t)];
            }
        }
    }

    seq_t ans = dp[6 * cols + n];
    seq_free_array(dp);
    return ans;
}

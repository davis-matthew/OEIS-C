/*
 * A319820  Number of partitions of n into exactly ten positive triangular
 *          numbers.
 * https://oeis.org/A319820
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 117, u16 n <= 642, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of partitions of n into exactly ten positive triangular numbers.
 * Computed by dynamic programming using the generating function
 *   Product_{j>=1} (1 - y*x^{j(j+1)/2})^{-1}
 * and selecting coefficient of y^10 x^n. */
#include <stdlib.h>
#include <stdint.h>

/* External linkage; seq_t, idx_t types are defined by the build system. */
seq_t a(idx_t n)
{
    /* For n below the offset we return 0; behaviour is irrelevant. */
    if (n < 10) return (seq_t)0;

    /* Allocate dynamic programming table: dp[k][s] = number of ways to
     * partition s into k triangular numbers (unordered).  Use a 1D array
     * for flat indexing. */
    size_t stride = (size_t)(n + 1);
    size_t total  = (size_t)11 * stride;
    seq_t *dp = (seq_t*)calloc(total, sizeof(seq_t));
    if (!dp) return (seq_t)0; /* allocation failure; unlikely */

    /* Base case: one way to partition 0 using 0 parts. */
    dp[0] = (seq_t)1; /* dp[0][0] */

    /* Enumerate triangular numbers t = i(i+1)/2 <= n. */
    for (idx_t i = 1;; ++i) {
        idx_t t = (i * (i + 1)) / 2;
        if (t > n) break;

        /* For each triangular number, update DP for k = 1..10
         * and sums s from t to n (ascending to allow unlimited copies). */
        for (int k = 1; k <= 10; ++k) {
            size_t k_offset   = (size_t)k * stride;
            size_t k1_offset  = (size_t)(k - 1) * stride;
            for (idx_t s = t; s <= n; ++s) {
                dp[k_offset + s] += dp[k1_offset + (s - t)];
            }
        }
    }

    seq_t result = dp[(size_t)10 * stride + n];
    free(dp);
    return result;
}

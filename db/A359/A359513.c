/*
 * A359513  Number of partitions of n into at most 4 positive Fibonacci
 *          numbers (with a single type of 1).
 * https://oeis.org/A359513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into at most 4 positive
 *            Fibonacci numbers * (with a single type of 1). Computed via
 *            dynamic programming that counts * unordered partitions using the
 *            Fibonacci part set up to n
 * Range:     u8 n <= 9626, u16 n <= 9626, u32 n <= 9626, u64 n <= 9626,
 *            u128 n <= 9626, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Generate Fibonacci numbers <= n, with 1 appearing only once */
    idx_t fibs[25];
    int fibc = 0;
    if (n >= 1) fibs[fibc++] = 1;
    if (n >= 2) fibs[fibc++] = 2;
    while (fibc >= 2) {
        idx_t next = fibs[fibc - 1] + fibs[fibc - 2];
        if (next > n) break;
        fibs[fibc++] = next;
    }

    /* Allocate DP table: dp[k][s] = number of partitions of s using exactly k parts */
    seq_t **dp = (seq_t **)malloc(5 * sizeof(seq_t *));
    for (int k = 0; k < 5; ++k) {
        dp[k] = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    }
    dp[0][0] = (seq_t)1;          /* one way to partition 0 with 0 parts */

    /* Dynamic programming over Fibonacci parts */
    for (int fi = 0; fi < fibc; ++fi) {
        idx_t f = fibs[fi];
        for (int k = 1; k <= 4; ++k) {
            for (idx_t s = f; s <= n; ++s) {
                dp[k][s] += dp[k - 1][s - f];
            }
        }
    }

    /* Sum partitions with <=4 parts */
    seq_t result = (seq_t)0;
    for (int k = 0; k <= 4; ++k) {
        result += dp[k][n];
    }

    /* Free DP table */
    for (int k = 0; k < 5; ++k) {
        free(dp[k]);
    }
    free(dp);

    return result;
}

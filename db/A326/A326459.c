/*
 * A326459  Sum of the fifth largest parts of the partitions of n into 8
 *          primes.
 * https://oeis.org/A326459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A326459
   Formula:   a(n) = Sum over all unordered partitions of n into 8 primes of
               the 4th smallest (i.e., 5th largest) part. */

/* The computation uses dynamic programming over primes:
 *   dp[k][s]   – number of partitions of s into k primes   (k = 0..8)
 *   sum4[k][s] – sum of the 4th smallest part over those partitions
 *                (defined only for k ≥ 4, otherwise 0).
 *
 * Adding a prime p to an existing partition of s-p with k‑1 parts
 * creates a new partition of s with k parts.  The 4th smallest part
 * in the new partition is:
 *   • p   if k = 4,
 *   • unchanged (the previous 4th smallest) for k ≥ 5;
 *   • undefined for k ≤ 3.
 *
 * Thus the following recurrence holds:
 *   dp[k][s]   += dp[k-1][s-p];
 *   sum4[k][s] += (k==4 ? dp[k-1][s-p] * p
 *                       : k>=5 ? sum4[k-1][s-p]
 *                              : 0);
 *
 * Finally, a(n) = sum4[8][n].
 */

#include <stdlib.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* Handle the trivial case n = 0 explicitly to avoid allocating zero‑sized arrays. */
    if (n == 0)
        return (seq_t)0;

    /* Build the list of primes ≤ n. */
    size_t primes_cnt = 0;
    uint64_t *primes = NULL;
    build_primes_upto(n, &primes, &primes_cnt);
    if (primes_cnt == 0)
        return (seq_t)0;

    /* Allocate DP tables. */ 
    size_t sz = (size_t)n + 1;       /* indices 0..n */
    seq_t **dp   = (seq_t **)malloc(9 * sizeof(seq_t *));
    seq_t **sum4 = (seq_t **)malloc(9 * sizeof(seq_t *));
    for (size_t k = 0; k <= 8; ++k) {
        dp[k]   = (seq_t *)calloc(sz, sizeof(seq_t));
        sum4[k] = (seq_t *)calloc(sz, sizeof(seq_t));
    }

    dp[0][0] = (seq_t)1;            /* Empty partition */

    /* Dynamic programming over the prime list. */
    for (size_t pi = 0; pi < primes_cnt; ++pi) {
        uint64_t p_val = primes[pi];
        seq_t p       = (seq_t)p_val;    /* cast for arithmetic */

        for (size_t k = 1; k <= 8; ++k) {
            for (size_t s = p_val; s <= (size_t)n; ++s) {
                seq_t prevCnt = dp[k-1][s - p_val];
                if (prevCnt == 0)
                    continue;           /* no partition to extend */

                /* Update number of partitions. */
                dp[k][s] += prevCnt;

                /* Update weighted sum of the 4th smallest part. */
                if (k == 4) {
                    /* new 4th smallest is the added prime p */
                    sum4[k][s] += prevCnt * p;
                } else if (k >= 5) {
                    /* 4th smallest unchanged */
                    sum4[k][s] += sum4[k-1][s - p_val];
                }
                /* for k <= 3 nothing to add */
            }
        }
    }

    seq_t result = sum4[8][n];

    /* Free allocated memory. */
    for (size_t k = 0; k <= 8; ++k) {
        free(dp[k]);
        free(sum4[k]);
    }
    free(dp);
    free(sum4);
    free(primes);

    return result;
}

/*
 * A319817  Number of partitions of n into exactly seven positive triangular
 *          numbers.
 * https://oeis.org/A319817
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 180, u16 n <= 1986, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A319817: number of partitions of n into exactly seven positive triangular numbers.
   Computed by dynamic programming:  dp[p][s] = number of ways to write
   s as a sum of p triangular numbers.  Uses a 2‑dimensional table that is
   rebuilt only when a larger n is requested.  The table is stored
   in statically allocated memory; all values are of type seq_t. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* external integer types supplied by the build */
extern seq_t a(idx_t n);

/* static storage for the DP table */
static seq_t **dp = NULL;          /* dp[p][s] : 0 ≤ p ≤ 7, 0 ≤ s ≤ current_max */
static idx_t current_max = 0;       /* maximum n for which the table is built */

/* free and rebuild the DP table up to limit (inclusive) */
static void build_dp(idx_t limit)
{
    /* free any previous table */
    if (dp) {
        for (size_t p = 0; p <= 7; ++p) {
            free(dp[p]);
        }
        free(dp);
        dp = NULL;
        current_max = 0;
    }

    /* allocate new table */
    dp = (seq_t **)malloc((7 + 1) * sizeof(seq_t *));
    if (!dp) abort();

    for (size_t p = 0; p <= 7; ++p) {
        dp[p] = (seq_t *)calloc((size_t)(limit + 1), sizeof(seq_t));
        if (!dp[p]) abort();
    }

    /* base case: 0 parts sum to 0 in exactly 1 way */
    dp[0][0] = 1;

    /* dynamic programming over triangular numbers t_k = k*(k+1)/2 */
    for (idx_t k = 1; ; ++k) {
        idx_t t = (k * (k + 1)) / 2;     /* triangular number, still fits idx_t */
        if (t > limit) break;
        for (size_t p = 1; p <= 7; ++p) {
            for (idx_t s = t; s <= limit; ++s) {
                dp[p][s] += dp[p - 1][s - t];
            }
        }
    }

    current_max = limit;
}

/* public function requested by the build */
seq_t a(idx_t n)
{
    /* any n < 7 has no partitions into 7 positive triangular numbers */
    if (n < 7) return 0;

    if (n > current_max || dp == NULL) {
        build_dp(n);
    }

    return dp[7][n];
}

/*
 * A342249  Number of partitions of n into nine powers of 2.
 * https://oeis.org/A342249
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn
 * Formula:   number of partitions of n into exactly nine powers of 2. * a(n)
 *            = number of multisets {p1,…,p9} of powers of 2 with p1+…+p9 = n
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, calloc, free */

static seq_t *dp = NULL;          /* 1‑D array holding a 10×(max_n+1) table */
static idx_t max_n = 0;           /* largest n for which dp is valid */

/* Compute dp table up to given limit.  Cached across calls. */
static void compute_dp(idx_t limit)
{
    /* Reset/allocate table for the new limit */
    if (limit < max_n) return;            /* already computed */

    /* Free old table if any */
    if (dp) free(dp);

    /* Allocate new table: 10 rows × (limit+1) columns */
    size_t rows    = 10;
    size_t cols    = (size_t)(limit + 1);
    size_t size    = rows * cols;
    dp = (seq_t *)calloc(size, sizeof(seq_t));   /* zero‑initialised */
    if (!dp) return;               /* memory failure → leave dp NULL */

    /* Helper macro to index dp[k][s] */
    #define DP(k,s) ((dp)[(size_t)(k) * cols + (size_t)(s)])

    /* Base case */
    DP(0,0) = (seq_t)1;

    /* Iteratively add powers of 2 (unlimited supply) */
    for (idx_t coin = 1; coin <= limit; coin <<= 1) {
        for (idx_t k = 1; k <= 9; ++k) {
            for (idx_t s = coin; s <= limit; ++s) {
                DP(k,s) += DP(k-1, s-coin);
            }
        }
    }

    max_n = limit;   /* table now valid up to new limit */
    #undef DP
}

seq_t a(idx_t n)
{
    if (n < 9) return (seq_t)0;        /* offset behaviour is irrelevant */

    compute_dp(n);                     /* ensure table covers n */
    if (!dp) return (seq_t)0;           /* allocation failed */

    /* Index into dp[9][n] */
    size_t cols = (size_t)(max_n + 1);
    return dp[9 * cols + n];
}

/*
 * A279221  Expansion of Product_{k>=1} 1/(1 - x^(k^2*(k+1)/2)).
 * https://oeis.org/A279221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A279221: number of partitions of n into pentagonal pyramidal numbers
   (A002411).  The function computes this value by dynamic programming
   enumerating the available coin values for the given n.  Each value of n
   is computed independently, so calls may be in any order.  For
   arbitrary‑precision builds seq_t is assumed to support the usual
   integer operators.  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate space for dp[0..n].  Use size_t for the allocation size.  */
    size_t len = (size_t)n + 1;
    seq_t *dp = (seq_t *)malloc(len * sizeof(seq_t));
    if (!dp) abort();          /* allocation failed */

    /* Initialise dp array.  All entries are zero except dp[0] = 1. */
    for (size_t i = 0; i < len; ++i) dp[i] = 0;
    dp[0] = 1;

    /* Generate the list of pentagonal pyramidal numbers <= n
       and perform the classic coin‑change DP. */
    for (idx_t k = 1; ; ++k)
    {
        seq_t coin = (seq_t)k * (seq_t)k * (seq_t)(k + 1) / 2;  /* k^2*(k+1)/2 */
        if (coin > n) break;            /* coin values are monotone */
        idx_t c = (idx_t)coin;          /* safe cast: coin <= n */
        for (idx_t i = c; i <= n; ++i)
            dp[i] += dp[i - c];
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}

/*
 * A357382  Expansion of Product_{k>=1} (1 - x^Lucas(k)).
 * https://oeis.org/A357382
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥1} (1 - x^{Lucas(k)})
 * Range:     i8 n <= 91, i16 n <= 91, i32 n <= 91, i64 n <= 91, i128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is the signed number of partitions of n into distinct Lucas numbers. */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate DP array of size n+1 */
    size_t size = (size_t)(n + 1U);
    seq_t *dp = (seq_t *)malloc(size * sizeof(seq_t));
    if (!dp) return 0;          /* fallback on allocation failure */

    dp[0] = 1;
    for (size_t i = 1; i < size; ++i) dp[i] = 0;

    seq_t limit = (seq_t)n;

    /* Lucas numbers: L0 = 2, L1 = 1, then Lk = L{(k-1)} + L{(k-2)} */
    seq_t lp = 1;    /* current Lucas number (L1) */
    seq_t lpprev = 2;/* previous Lucas number (L0) */

    while (lp <= limit) {
        size_t lps = (size_t)lp;
        for (size_t s = n; s >= lps; --s) {
            dp[s] -= dp[s - lps];
        }
        seq_t next = lpprev + lp;
        lpprev = lp;
        lp = next;
    }

    seq_t result = dp[(size_t)n];
    free(dp);
    return result;
}

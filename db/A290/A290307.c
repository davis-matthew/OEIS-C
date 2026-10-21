/*
 * A290307  Square array A(n,k), n >= 0, k >= 1, read by antidiagonals, where
 *          column k is the expansion of Product_{j>=1} (1 + x^j)/(1 +
 *          x^(k*j)).
 * https://oeis.org/A290307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 811, u16 n <= 4547, u32 n <= 9869, u64 n <= 9869,
 *            u128 n <= 9869, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of partitions of i into distinct parts that are not
 * multiples of k, where (i,k) is the entry on the n‑th antidiagonal of the
 * square array A(n,k).  The array A(n,k) enumerates partitions of n into
 * distinct parts with no part divisible by k.  The function computes the
 * required entry using a simple dynamic program over distinct parts.
 *
 * Comment:  The indices i and k are derived from n by locating the n‑th
 * element when the array is read by antidiagonals: let d be the
 * smallest integer with d(d+1)/2 > n.  Then i = n - d(d-1)/2 and
 * k = d - i + 1.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find the diagonal index d such that d(d+1)/2 <= n < (d+1)(d+2)/2 */
    idx_t d = 0;
    while (((d + 1) * (d + 2)) / 2 <= n)
        d++;

    /* Determine row (i) and column (k) on this diagonal */
    idx_t start = d * (d + 1) / 2;    /* first index on diagonal d   */
    idx_t i = n - start;              /* row index, 0 <= i <= d       */
    idx_t k = d + 1 - i;              /* column index (k >= 1)        */

    /* DP array for partitions of i into distinct parts not divisible by k */
    size_t m = (size_t)i;
    seq_t *dp = (seq_t*)calloc(m + 1, sizeof(seq_t));
    if (!dp) return (seq_t)0;          /* out of memory (unlikely)    */
    dp[0] = (seq_t)1;                  /* empty partition             */

    for (idx_t part = 1; part <= m; ++part) {
        if (part % k == 0)             /* forbidden part              */
            continue;
        for (idx_t val = m; val >= part; --val) {
            dp[val] += dp[val - part];
        }
    }

    seq_t result = dp[m];
    free(dp);
    return result;
}

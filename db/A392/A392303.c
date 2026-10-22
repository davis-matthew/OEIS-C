/*
 * A392303  Number of NE lattice paths from (0,0) to (n,n) staying weakly
 *          below the discretized parabolic boundary y = floor(x^2/n).
 * https://oeis.org/A392303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of NE lattice paths from (0,0) to (n,n) staying
 *            weakly below the discretized parabolic boundary y = floor(x^2 /
 *            n). The function implements the DP recurrence N(x,y) = N(x-1,y)
 *            + N(x,y-1) with boundary conditions N(x,y)=0 if y > floor(x^2/n)
 *            or x<0 or y<0, N(0,0)=1, and returns a(n)=...
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 23, u64 n <= 42, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the trivial case immediately. */
    if (n == 0)
        return (seq_t)1;                      /* a(0) = 1 */

    /* Allocate a one‑dimensional array of length n+1 to hold
       N(x,y) values for the current x.  Only the current row
       is kept during the DP process. */
    const size_t sz = (size_t)n + 1;
    seq_t *dp = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!dp)
        return (seq_t)0;                      /* out‑of‑memory fallback */

    dp[0] = (seq_t)1;                         /* N(0,0) = 1 */
    idx_t prev_limit = 0;                     /* boundary height for x-1 */

    for (idx_t x = 1; x <= n; ++x) {
        /* Compute the boundary height for this x. */
        seq_t tmp        = ((seq_t)x * (seq_t)x) / (seq_t)n;
        idx_t lim        = (idx_t)tmp;
        if (lim > n)            /* safety guard */
            lim = n;

        /* Update all y positions that are within the new boundary. */
        for (size_t y = 0; y <= (size_t)lim; ++y) {
            seq_t up   = dp[y];                 /* N(x-1, y) from previous row   */
            seq_t left = (y > 0) ? dp[y-1] : (seq_t)0; /* N(x, y-1) after update  */
            dp[y] = up + left;
        }

        /* Zero entries that have moved above the boundary. */
        if (prev_limit > lim) {
            for (idx_t y = lim + 1; y <= prev_limit; ++y)
                dp[(size_t)y] = 0;
        }
        prev_limit = lim;
    }

    seq_t result = dp[(size_t)n];
    free(dp);
    return result;
}

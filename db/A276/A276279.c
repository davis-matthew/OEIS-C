/*
 * A276279  Number of triangular partitions of n of order 8.
 * https://oeis.org/A276279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A276279: Number of triangular partitions of n of order 8
 *  Generating function: 1/((1-x)^8*(1-x^3)^7*(1-x^5)^6*(1-x^7)^5*(1-x^9)^4*(1-x^11)^3*(1-x^13)^2*(1-x^15))
 *  The function implements this as a convolution of 36 unbounded coin types with
 *  weights 1,3,5,7,9,11,13,15 having multiplicities 8,7,6,5,4,3,2,1 respectively.
 *  For each coin type we apply the usual unbounded‑coin DP update:
 *      dp[s] += dp[s - weight]
 *  Consequently dp[s] equals the coefficient of x^s in the product and is the desired
 *  term a(s). */

#include <stdlib.h>

/* Array of the 36 coin weights corresponding to the factors in the generating function.
 * 8 copies of 1, 7 copies of 3, 6 copies of 5, 5 copies of 7,
 * 4 copies of 9, 3 copies of 11, 2 copies of 13 and 1 copy of 15. */
static const idx_t weights[36] = {
     1, 1, 1, 1, 1, 1, 1, 1,
     3, 3, 3, 3, 3, 3, 3,
     5, 5, 5, 5, 5, 5,
     7, 7, 7, 7, 7,
     9, 9, 9, 9,
    11, 11, 11,
    13, 13,
    15
};

/* ------------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Allocate an array of seq_t of size n+1 initialized to 0 */
    seq_t *dp = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
    if (dp == NULL) {
        /* In an environment where malloc can fail, return 0 for safety */
        return 0;
    }

    dp[0] = 1;  /* coefficient of x^0 is 1 */

    /* Iterate over the 36 coin types and apply the unbounded‑coin DP update */
    for (size_t t = 0; t < 36; ++t) {
        idx_t w = weights[t];
        for (idx_t s = w; s <= n; ++s) {
            dp[s] += dp[s - w];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}

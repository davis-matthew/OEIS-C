/*
 * A274313  The number of conjugacy classes of n X n matrices over GF(2) which
 *          are squares of other such matrices.
 * https://oeis.org/A274313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the coefficient of z^n in \prod_{m odd \lor m \equiv 0
 *            \pmod 4} (1 - 2 z^m)^{-1}. This is computed directly by dynamic
 *            programming that adds a part of size m with weight 2 for each
 *            allowed m
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n) {
    /* Allocate array for DP up to n.  Use calloc to zero initialise. */
    size_t len = (size_t)(n + 1);
    seq_t *dp = (seq_t *)calloc(len, sizeof(seq_t));
    if (!dp) {            /* fallback if allocation fails */
        return (seq_t)0;
    }

    dp[0] = (seq_t)1;      /* Base case: empty partition */

    /* Process all allowed part sizes up to n */
    for (idx_t m = 1; m <= n; ++m) {
        /* m is odd or divisible by 4 */
        if ( (m & 1) || ((m & 3) == 0) ) {
            for (idx_t s = m; s <= n; ++s) {
                seq_t tmp = dp[s - m];
                tmp = tmp + tmp;          /* multiply by 2 */
                dp[s] = dp[s] + tmp;      /* add contribution */
            }
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}

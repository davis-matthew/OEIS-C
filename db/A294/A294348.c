/*
 * A294348  Sum of products of terms in all partitions of 10*n into powers of
 *          10.
 * https://oeis.org/A294348
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the products of all parts in every partition of
 *            10*n * into powers of 10. The generating function is * Π_{i≥0}
 *            (1 - 10^i z^{10^i})^{-1} * and a(n) is the coefficient of
 *            z^{10n}. The algorithm uses the * standard unbounded–coin DP and
 *            runs in O(10n * log10(10n)) time
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* The target sum is m = 10 * n.  All indices are small enough for
       idx_t and size_t.  */
    idx_t m = n * 10;                      /* exact multiplication in seq_t */
    size_t maxS = (size_t)m;               /* size of DP array */

    /* Allocate the DP array and initialise.  Memory is freed before returning. */
    seq_t *dp = (seq_t *)malloc((maxS + 1) * sizeof(seq_t));
    if (!dp) return 0;                     /* allocation failure – return 0 */
    for (size_t i = 1; i <= maxS; ++i) dp[i] = 0;
    dp[0] = 1;                              /* empty partition has product 1 */

    /* For each power of ten 10^i (value = pow), update dp using the
       recurrence:
                 dp[s] += dp[s - pow] * 10^i  (for s >= pow)
       This accounts for adding one part of size pow and any number of
       preceding parts of the same value. */
    for (idx_t i = 0, pow = 1; pow <= maxS; ++i, pow *= 10) {
        seq_t factor = pow10((idx_t)i);   /* 10^i in seq_t form */
        for (size_t s = (size_t)pow; s <= maxS; ++s) {
            dp[s] += dp[s - (size_t)pow] * factor;
        }
    }

    seq_t result = dp[maxS];               /* a(n)  */
    free(dp);
    return result;
}

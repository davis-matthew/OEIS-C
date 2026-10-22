/*
 * A327367  Number of labeled simple graphs with n vertices, at least one of
 *          which is isolated.
 * https://oeis.org/A327367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} (-1)^{k-1} C(n,k) · 2^{(n-k)(n-k-1)/2} (Number
 *            of labeled graphs on n vertices that have at least one isolated
 *            vertex)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* a(0) = 0 */

    seq_t sum = 0;                          /* running total */

    /* initial exponent for k = 1 : (n-1)(n-2)/2 */
    idx_t exp = (n - 1) * (n - 2) / 2;       /* fits in idx_t for n ≤ 10000 */
    seq_t pow2 = pow_int((seq_t)2, exp);     /* 2^exp */

    seq_t binom = 1;                         /* C(n,0) ; will be updated to C(n,1) in the loop */
    for (idx_t k = 1; k <= n; ++k)
    {
        /* update binomial coefficient to C(n,k) */
        binom = binom * (n - k + 1) / k;

        /* term = C(n,k) * 2^{(n-k)(n-k-1)/2} */
        seq_t term = binom * pow2;

        /* add or subtract according to (-1)^{k-1} */
        if (k & 1)   /* k odd -> + */
            sum += term;
        else        /* k even -> - */
            sum -= term;

        /* prepare for next iteration (k+1) */
        if (k < n)
        {
            /* next exponent decreases by (n - (k+1)) */
            seq_t divisor = pow_int((seq_t)2, n - k - 1);
            pow2 = pow2 / divisor;
        }
    }

    return sum;
}

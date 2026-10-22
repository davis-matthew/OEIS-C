/*
 * A302117  a(n) = 4*(n-1)*a(n-1) - (1/3)*Product_{k=0..n-1} (2*k-3), with
 *          a(0) = 0.
 * https://oeis.org/A302117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (recurrence): a(0) = 0, a(1) = 1
   a(n) = 4*(n-1)*a(n-1) - prod(n),  n >= 2
   where prod(n) = ∏_{k=1}^{n-1} |2k-3|  (positive integers) */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    seq_t ans = (seq_t)1;   /* a(1) */
    seq_t prod = (seq_t)1;  /* prod(2) = |2*1-3| = 1 */

    for (idx_t i = 2; i <= n; ++i)
    {
        /* term = 4*(i-1)*ans   (all in seq_t arithmetic) */
        seq_t term = 4 * (seq_t)(i - 1) * ans;
        ans = term - prod;          /* a(i) = 4*(i-1)*a(i-1) - prod */

        /* Update prod for next iteration: multiply by |2*i-3| */
        int64_t fac = 2 * (int64_t)i - 3;
        if (fac < 0)
            fac = -fac;
        prod = prod * (seq_t)fac;
    }

    return ans;
}

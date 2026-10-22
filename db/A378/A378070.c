/*
 * A378070  a(n) = binomial(n - 1, ceiling(n/2)) * binomial(n - 1,
 *          ceiling(n/2) - 1).
 * https://oeis.org/A378070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n - 1, ceil(n/2)) * binomial(n - 1, ceil(n/2) -
 *            1)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 35, u128 n <= 68,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;                 /* a(0) = 1 */

    idx_t nn = n - 1;                    /* n - 1 (n ≥ 1 so no underflow) */
    idx_t k  = (n + 1) / 2;              /* ceil(n/2) = (n+1)/2 for integer n */

    /* binom(a, b) is defined in utils/binom.h; it returns 0 if b > a */
    return binom(nn, k) * binom(nn, k - 1);
}

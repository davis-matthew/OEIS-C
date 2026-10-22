/*
 * A301898  a(n) = (2*n + 1)! if n is even, a(n) = 2*(2*n + 1)! if n is odd.
 * https://oeis.org/A301898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*n + 1)! if n is even, 2*(2*n + 1)! if n is odd
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   factorial, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"    /* factoriαl(idx_t) -> seq_t */
#include "../utils/mul_small.h"    /* mul_small(seq_t, unsigned int) -> seq_t */

seq_t a(idx_t n)
{
    /* compute (2*n + 1)! */
    idx_t m = (n << 1) + 1;           /* 2*n + 1  (shift is safe for idx_t) */
    seq_t val = factorial(m);

    /* if n is odd, multiply by 2 */
    if (n & 1U)
        val = mul_small(val, 2U);

    return val;
}

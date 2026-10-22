/*
 * A373386  Smallest integer m > 1 such that m == m^m (mod 10^(len(m) + n)),
 *          where len(m) is the number of digits of m.
 * https://oeis.org/A373386
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(0)=5, a(1)=51, a(2)=751; for n>=3, a(n)=10^(n+1)+1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Special small indices */
    if (n == 0) return 5;
    if (n == 1) return 51;
    if (n == 2) return 751;

    /* General case: 10^(n+1) + 1 */
    idx_t exp = n + 1;
    seq_t p = pow_uint(10, exp);
    return p + 1;
}

/*
 * A373279  Expansion of Sum_{k>=0} x^(3^k) / (1 - 3*x^(3^k)).
 * https://oeis.org/A373279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^(n-1) if n % 3 != 0; a(n) = 3^(n-1) + a(n/3) if n % 3
 *            == 0
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Base: for n >= 1 */
    seq_t res = pow3(n - 1);
    if (n % 3 == 0)
        res += a(n / 3);
    return res;
}

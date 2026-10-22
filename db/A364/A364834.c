/*
 * A364834  Sum of positive integers <= n which are multiples of 2 or 5.
 * https://oeis.org/A364834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sn(n,2) + sn(n,5) - sn(n,10) * where sn(n,d) = (an(n,d) *
 *            (an(n,d) + d)) / (2*d) * and an(n,d) = d * (n/d)
 * Range:     u8 n <= 29, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   sn
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sn.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;
    seq_t result = sn(n, 2);
    result += sn(n, 5);
    result -= sn(n, 10);
    return result;
}

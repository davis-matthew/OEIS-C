/*
 * A332139  a(n) = (10^(2*n+1)-1)/3 + 6*10^n.
 * https://oeis.org/A332139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (10^(2*n+1)-1)/3 + 6*10^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t t1 = pow10(2*n + 1);
    t1 = (t1 - 1) / 3;
    seq_t t2 = pow10(n);
    t2 *= 6;
    return t1 + t2;
}

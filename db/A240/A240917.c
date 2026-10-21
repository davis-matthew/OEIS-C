/*
 * A240917  a(n) = 2*3^(2*n) - 3*3^n + 1.
 * https://oeis.org/A240917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*3^(2*n) - 3*3^n + 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    seq_t pow_n = pow3(n);       /* 3^n */
    seq_t pow_2n = pow_n * pow_n;/* 3^(2*n) = (3^n)^2 */
    seq_t res = 2 * pow_2n;
    res -= 3 * pow_n;
    res += 1;
    return res;
}

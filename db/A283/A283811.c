/*
 * A283811  Poly-Bernoulli numbers B_n^(k) with k = -5.
 * https://oeis.org/A283811
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 120*6^n - 240*5^n + 150*4^n - 30*3^n + 2^n
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* compute base powers */
    seq_t p2 = pow_uint(2, n);
    seq_t p3 = pow_uint(3, n);
    seq_t p4 = pow_uint(4, n);
    seq_t p5 = pow_uint(5, n);
    seq_t p6 = pow_uint(6, n);

    /* combine terms with positive and negative signs separately
       to avoid unsigned subtraction wrap‑around */
    seq_t positive = 120 * p6 + 150 * p4 + p2;
    seq_t negative = 240 * p5 + 30 * p3;

    return positive - negative;
}

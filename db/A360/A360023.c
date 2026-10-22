/*
 * A360023  Expansion of e.g.f. x*exp(x)*(cosh(x))^2.
 * https://oeis.org/A360023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(3^(n-1)+(-1)^(n-1)+2)/4
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t pow3 = big_pow((seq_t)3, (seq_t)(n - 1));
    seq_t add  = (((n - 1) & 1) == 0U) ? (seq_t)3 : (seq_t)1;
    seq_t sum  = pow3 + add;
    seq_t sum_div4 = sum >> 2;          /* sum is always divisible by 4 */
    return ((seq_t)n) * sum_div4;
}

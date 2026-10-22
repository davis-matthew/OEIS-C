/*
 * A360951  Expansion of e.g.f. (cosh(x) - 1)*(1 + x)*exp(x).
 * https://oeis.org/A360951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n + 2) * 2^(n-2) - n - 1, with a(0) = a(1) = 0
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 29, u64 n <= 60, u128 n <= 123,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;

    seq_t pow_n_1 = pow2(n - 1);   /* 2^(n-1) */
    seq_t pow_n_2 = pow2(n - 2);   /* 2^(n-2) */
    seq_t res = pow_n_1 + (seq_t)n * pow_n_2;
    res -= (seq_t)n;
    res -= (seq_t)1;
    return res;
}

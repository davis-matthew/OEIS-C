/*
 * A377399  Expansion of e.g.f. (2 - exp(x))^4.
 * https://oeis.org/A377399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1 a(n) = 4^n - 8*3^n + 3*2^(n+3) - 32 (n > 0)
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 15, i64 n <= 31, i128 n <= 63,
 *            big any n
 * Helpers:   mul_small, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    seq_t pow4 = pow_seq((seq_t)4, n);
    seq_t pow3 = pow_seq((seq_t)3, n);
    seq_t pow2 = pow_seq((seq_t)2, n + 3);

    seq_t term1 = pow4;
    seq_t term2 = mul_small(pow3, 8);
    term2 = -term2;
    seq_t term3 = mul_small(pow2, 3);
    seq_t term4 = (seq_t)-32;

    seq_t result = term1 + term2 + term3 + term4;
    return result;
}

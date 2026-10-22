/*
 * A397570  Number of permutations p of [n+3] such that |p(i+1)-p(i)| <> n for
 *          all 1 <= i < n+3.
 * https://oeis.org/A397570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..3} (-2)^k * binomial(3,k) * (n+3-k)! for n >= 3
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   factorial, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Special cases for n = 0, 1, 2 are 6, 2, 28 respectively. */

#include "../utils/factorial.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)6;
    if (n == 1u) return (seq_t)2;
    if (n == 2u) return (seq_t)28;

    /* coefficients: 1, -6, 12, -8   (binomial(3,k) * (-2)^k) */
    static const int coeff[4] = { 1, -6, 12, -8 };

    seq_t pos = 0;
    seq_t neg = 0;

    for (int k = 0; k < 4; ++k) {
        /* m = n + (3 - k) */
        idx_t diff = 3u - (idx_t)k;
        idx_t m    = n + diff;

        seq_t f = factorial(m);

        unsigned unsigned_coeff = (unsigned)(coeff[k] > 0 ? coeff[k] : -coeff[k]);

        /* term = f * |coeff| */
        seq_t term = mul_small(f, unsigned_coeff);

        if (coeff[k] > 0)
            pos += term;
        else
            neg += term;
    }
    return pos - neg;
}

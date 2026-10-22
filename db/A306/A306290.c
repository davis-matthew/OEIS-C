/*
 * A306290  a(n) = 1/(Integral_{x=0..1} (x^3 - x^4)^n dx).
 * https://oeis.org/A306290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n+1) * binomial(4*n, n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute binomial(4*n, n) */
    seq_t res = binomial((idx_t)(4 * n), n);

    /* Multiply by (4*n + 1) */
    seq_t mul = (seq_t)(4 * n) + (seq_t)1;
    return res * mul;
}

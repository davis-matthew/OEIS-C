/*
 * A370286  Coefficient of x^n in the expansion of ( (1+x)^3 + x^2 )^n.
 * https://oeis.org/A370286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} binomial(n,k) * binomial(3*n-3*k, n-2*k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the provided helper binomial function and works for
   any seq_t (fixed-width or arbitrary precision). */

#include "../utils/binomial.h"  /* assumes binomial is available via utils */

seq_t a(idx_t n)
{
    idx_t maxk = n >> 1;          /* floor(n/2) */
    seq_t sum = 0;

    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t term1 = binomial(n, k);
        seq_t term2 = binomial(3 * n - 3 * k, n - 2 * k);
        sum += term1 * term2;
    }
    return sum;
}

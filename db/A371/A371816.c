/*
 * A371816  a(n) = Sum_{k=0..floor(n/3)} (-1)^k * binomial(3*n-3*k-1,n-3*k).
 * https://oeis.org/A371816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..⌊n/3⌋} (-1)^k * binomial(3*n-3*k-1, n-3*k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* The only case where the top argument of the binomial is negative
       is n = 0.  For n = 0 the sum is defined to be 1. */
    if (n == 0)
        return (seq_t)1;

    seq_t sum   = 0;
    idx_t maxk  = n / 3;
    for (idx_t k = 0; k <= maxk; ++k) {
        /* Compute 3*n - 3*k - 1 and n - 3*k explicitly in idx_t */
        idx_t top    = 3 * n - 3 * k - (idx_t)1;
        idx_t bottom = n - 3 * k;
        seq_t term   = binomial(top, bottom);
        if (k & 1U)
            sum -= term;
        else
            sum += term;
    }
    return sum;
}

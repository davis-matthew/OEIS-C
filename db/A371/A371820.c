/*
 * A371820  a(n) = Sum_{k=0..floor(n/3)} (-1)^k * binomial(2*n+2,n-3*k).
 * https://oeis.org/A371820
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} (-1)^k * binomial(2*n+2,n-3*k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t maxk = n / 3;
    seq_t pos = 0;
    seq_t neg = 0;
    for (idx_t k = 0; k <= maxk; ++k)
    {
        idx_t m = n - 3 * k;
        seq_t term = binomial(2 * n + 2, m);
        if ((k & 1) == 0)
            pos += term;
        else
            neg += term;
    }
    return pos - neg;  /* pos ≥ neg for all n, so subtraction is safe */
}

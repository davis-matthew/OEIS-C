/*
 * A382300  a(n) = Sum_{k=0..floor(n/2)} (k+1) * binomial(2*k,2*n-4*k).
 * https://oeis.org/A382300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} (k+1) * binomial(2*k, 2*n-4*k)
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 37, u64 n <= 75, u128 n <= 153,
 *            big any n
 * Helpers:   binomial, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t limit = n / 2;
    for (idx_t k = 0; k <= limit; ++k)
    {
        idx_t top = 2 * k;
        idx_t bottom = 2 * n - 4 * k;
        seq_t bin = binomial(top, bottom);   /* binomial(2*k, 2*n-4*k) */
        /* (k+1) fits in unsigned int for all expected n */
        unsigned int kp = (unsigned int)(k + 1);
        seq_t term = mul_small(bin, kp);
        sum += term;
    }
    return sum;
}

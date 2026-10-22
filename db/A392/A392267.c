/*
 * A392267  a(n) = Sum_{k=0..floor(2*n/3)} (k+1) * binomial(k,2*n-3*k).
 * https://oeis.org/A392267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊2n/3⌋} (k+1) * binomial(k, 2n-3k)
 * Range:     u8 n <= 11, u16 n <= 24, u32 n <= 50, u64 n <= 103,
 *            u128 n <= 213, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* Upper bound k = floor(2n/3) */
    idx_t kmax = (2 * n) / 3;

    for (idx_t k = 0; k <= kmax; ++k)
    {
        idx_t t = 2 * n - 3 * k;          /* t is non‑negative for k ≤ kmax */
        seq_t b = binomial(k, t);        /* returns 0 if t > k */
        if (b != 0)
            sum += (seq_t)(k + 1) * b;
    }
    return sum;
}

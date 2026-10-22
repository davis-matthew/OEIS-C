/*
 * A383536  a(n) = Sum_{k=0..floor(n/2)} binomial(n+k,k)^2 * binomial(n-k,k).
 * https://oeis.org/A383536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(n+k,k)^2 * binomial(n-k,k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Source: A383536 – diagonal of 1 / ((1-x)*(1-y)*(1-z) - z^2) */

#include "../utils/binomial.h"

/* Computes the n-th term of sequence A383536. */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t kmax = n / 2;               /* floor(n/2) */
    for (idx_t k = 0; k <= kmax; ++k)
    {
        seq_t c1 = binomial(n + k, k);            /* C(n+k, k) */
        seq_t term = c1 * c1 * binomial(n - k, k);/* C(n+k,k)^2 * C(n-k,k) */
        sum += term;
    }
    return sum;
}

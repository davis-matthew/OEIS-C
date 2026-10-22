/*
 * A366042  Expansion of (1/x) * Series_Reversion( x*(1-x)*(1-x^5) ).
 * https://oeis.org/A366042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..floor(n/5)} binomial(n+k,n) *
 *            binomial(2*n-5*k,n)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute the sum of products of binomials for k = 0 .. floor(n/5) */
    idx_t maxk = n / 5;
    seq_t sum = 0;
    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t t1 = binomial(n + k, n);                /* binomial(n+k, n) */
        seq_t t2 = binomial((2 * n) - (5 * k), n);    /* binomial(2n-5k, n) */
        sum += t1 * t2;                               /* add product to sum */
    }

    /* Divide by (n+1), exact division */
    seq_t denom = ((seq_t)n) + 1;
    return sum / denom;
}

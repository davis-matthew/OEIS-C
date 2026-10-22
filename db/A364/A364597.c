/*
 * A364597  G.f. satisfies A(x) = 1/(1-x) + x^3*(1-x)*A(x)^5.
 * https://oeis.org/A364597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/3)}
 *            binomial(n,3*k)*binomial(5*k,k)/(4*k+1)
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 23, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 3;
    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t b1    = binomial(n, 3 * k);          /* binomial(n,3*k)   */
        seq_t b2    = binomial(5 * k, k);          /* binomial(5*k,k)   */
        seq_t denom = 4 * k + 1;                   /* divisor (4*k+1)   */
        seq_t temp  = b2 / denom;                  /* division is exact */
        seq_t term  = b1 * temp;                   /* multiply normal   */
        sum += term;
    }
    return sum;
}

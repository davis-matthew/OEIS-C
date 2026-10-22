/*
 * A376546  G.f. A(x) satisfies A(x) = 1 + x^4*(1+x)*A(x)^4.
 * https://oeis.org/A376546
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=0..⌊n/4⌋} binomial(k, n-4k) * binomial(4k, k)/(3k+1)
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 38, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 4;
    for (idx_t k = 0; k <= maxk; ++k)
    {
        idx_t m = n - 4 * k;
        if (m > k) continue;                     /* binomial(k,m) = 0 in this case */
        seq_t c1   = binomial(k, m);             /* binomial(k, n-4k) */
        seq_t c2   = binomial(4 * k, k);         /* binomial(4k, k)    */
        seq_t den  = (seq_t)(3 * k + 1);          /* denominator       */
        seq_t term = (c1 * c2) / den;             /* exact integer div */
        sum += term;
    }
    return sum;
}

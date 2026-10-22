/*
 * A368932  Expansion of (1/x) * Series_Reversion( x * (1-x) * (1-x-x^4) ).
 * https://oeis.org/A368932
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..floor(n/4)} binomial(n+k,k) *
 *            binomial(3*n-3*k+1,n-4*k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t maxk = n / 4;
    seq_t sum = (seq_t)0;

    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t t1 = binomial(n + k, k);                      /* C(n+k, k) */
        seq_t t2 = binomial((3 * n) - 3 * k + 1, n - 4 * k);/* C(3n-3k+1, n-4k) */
        sum += t1 * t2;                                     /* term */
    }

    return sum / ((seq_t)(n + 1));
}

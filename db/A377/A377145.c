/*
 * A377145  a(n) = Sum_{k=0..n} binomial(k+2,2) * binomial(k,n-k)^2.
 * https://oeis.org/A377145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(k+2,2) * binomial(k,n-k)^2
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        /* binomial(k+2,2) = (k+2)*(k+1)/2 */
        seq_t tri = ((seq_t)(k + 2) * (seq_t)(k + 1)) / 2;
        seq_t c   = binomial(k, n - k);       /* C(k, n-k) */
        seq_t term = tri * c * c;             /* tri * c^2 */
        sum += term;
    }
    return sum;
}

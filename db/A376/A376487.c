/*
 * A376487  G.f. satisfies A(x) = 1 / (1 - x^4*A(x)^4 * (1 + x)).
 * https://oeis.org/A376487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/4⌋} binom(5*k,k) * binom(k,n-4*k) / (4*k+1)
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 34, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 4;
    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t r = n - 4 * k;           /* n - 4*k */
        if (r > k)                     /* binom(k,r) would be zero */
            continue;

        seq_t term = binom(5 * k, k);  /* binom(5*k,k) */
        term *= binom(k, r);           /* * binom(k,r) */
        term /= (4 * k + 1);           /* / (4*k+1) */

        sum += term;
    }
    return sum;
}

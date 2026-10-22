/*
 * A366041  Expansion of (1/x) * Series_Reversion( x*(1-x)*(1-x^4) ).
 * https://oeis.org/A366041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..floor(n/4)} binomial(n+k,n) *
 *            binomial(2*n-4*k,n)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    idx_t m = n / 4;               /* maximum k */
    seq_t sum = 0;

    for (idx_t k = 0; k <= m; ++k) {
        seq_t t1 = binom(n + k, n);                    /* binomial(n+k, n) */
        seq_t t2 = binom(2 * n - 4 * k, n);            /* binomial(2n-4k, n) */
        sum += t1 * t2;
    }

    return sum / ((seq_t)n + 1);
}

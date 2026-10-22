/*
 * A366098  Expansion of (1/x) * Series_Reversion( x*(1+x-x^4)/(1+x)^2 ).
 * https://oeis.org/A366098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..floor(n/4)} binom(n+k,k) *
 *            binom(n-k+1,n-4*k)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t maxk = n / 4;
    seq_t sum = 0;

    for (idx_t k = 0; k <= maxk; ++k) {
        /* binom(n+k, k) */
        seq_t t1 = binomial(n + k, k);
        /* binom(n-k+1, n-4*k) */
        idx_t top   = n - k + 1;
        idx_t bottom = n - 4 * k;
        seq_t t2 = binomial(top, bottom);
        sum += t1 * t2;
    }

    return sum / (n + 1);
}

/*
 * A366054  Expansion of (1/x) * Series_Reversion( x*(1-x)^2/(1-x+x^4) ).
 * https://oeis.org/A366054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..floor(n/4)} binomial(n+1,k) *
 *            binomial(2*n-3*k, n-4*k)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 26 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t maxk = n / 4;
    seq_t sum = 0;
    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t c1 = binomial(n + 1, k);
        seq_t c2 = binomial(2 * n - 3 * k, n - 4 * k);
        sum += c1 * c2;
    }
    seq_t denom = (seq_t)(n + 1);
    return sum / denom;
}

/*
 * A369621  Expansion of (1/x) * Series_Reversion( x / (1/(1-x) + x^3) ).
 * https://oeis.org/A369621
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..floor(n/3)} binomial(n+1,k) * *
 *            binomial(2*n-4*k, n-3*k)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    const idx_t maxk = n / 3;
    seq_t sum = 0;

    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t term1 = binomial(n + 1, k);
        seq_t term2 = binomial(2 * n - 4 * k, n - 3 * k);
        sum += term1 * term2;
    }

    return sum / (seq_t)(n + 1);
}

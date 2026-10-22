/*
 * A369212  Expansion of (1/x) * Series_Reversion( x / ((1+x)^2+x^3) ).
 * https://oeis.org/A369212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A369212: a(n) = (1/(n+1)) * Sum_{k=0..floor(n/3)} binomial(n+1,k) *
 * binomial(2*n-2*k+2, n-3*k) */
#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    // upper bound for k
    idx_t maxk = n / 3;
    // precompute n+1 for reuse
    idx_t n_plus_1 = n + 1;
    seq_t sum = 0;
    for (idx_t k = 0; k <= maxk; ++k) {
        // first binomial: (n+1 choose k)
        seq_t b1 = binomial(n_plus_1, k);
        // second binomial: (2*n - 2*k + 2 choose n - 3*k)
        idx_t second_n = 2 * n - 2 * k + 2;
        idx_t second_k = n - 3 * k;
        seq_t b2 = binomial(second_n, second_k);
        sum += b1 * b2;
    }
    return sum / (n + 1);
}

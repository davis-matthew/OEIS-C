/*
 * A369399  Expansion of (1/x) * Series_Reversion( x / (1+x) * (1-x^3)^2 ).
 * https://oeis.org/A369399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..floor(n/3)} binomial(2*n+k+1, k) *
 *            binomial(n+1, n-3*k)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t max_k = n / 3;
    seq_t sum = 0;
    seq_t F = 1;                     /* F = C(2*n + k + 1, k) */

    for (idx_t k = 0; k <= max_k; ++k) {
        if (k > 0) {
            seq_t mult = (seq_t)(2 * n + k + 1);
            F = F * mult / (seq_t)k;
        }
        seq_t G = binomial(n + 1, 3 * k + 1);   /* G = C(n+1, 3*k+1) = C(n+1, n-3*k) */
        sum += F * G;
    }

    return sum / ((seq_t)n + 1);
}

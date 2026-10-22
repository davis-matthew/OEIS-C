/*
 * A379082  Expansion of (1/x) * Series_Reversion( x * (1/(1 + x) - x^3)^2 ).
 * https://oeis.org/A379082
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * Σ_{k=0}^{⌊n/3⌋} binomial(2*n + k + 2, k) *
 *            binomial(2*n + k + 2, n - 3*k) /(2*n + k + 2) where division is
 *            exact. This formula is taken from OEIS A379082
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 3;

    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t N = 2 * n + k + 2;                      /* N >= 2 */
        seq_t c1 = binomial(N, k);                    /* binomial(N, k)  */
        seq_t c2 = binomial(N, n - 3 * k);            /* binomial(N, n-3k) */
        seq_t term = c1 * c2;                         /* product fits in seq_t (arbitrary precision) */
        term = term * 2 / (seq_t)N;                   /* exact division by N */
        sum += term;                                  /* accumulate */
    }
    return sum;
}

/*
 * A381828  Expansion of ( (1/x) * Series_Reversion( x * ((1-x) * (1-x+x^2))^2
 *          ) )^(1/2).
 * https://oeis.org/A381828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(2*n+k+1, k) * binomial(3*n-2*k,
 *            n-k)/(2*n+k+1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses a multiplicative evaluation of the product in 128‑bit to avoid overflows
   when seq_t is 64‑bit; works correctly for all n for which the final result
   fits in seq_t. */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        idx_t a1 = 2 * n + k + 1;
        idx_t a2 = 3 * n - 2 * k;

        seq_t bin1 = binomial(a1, k);        /* C(a1, k)   */
        seq_t bin2 = binomial(a2, n - k);    /* C(a2, n-k) */

        /* product may overflow 64‑bit; compute in 128‑bit if available */
        __uint128_t prod = ( __uint128_t) bin1 * (__uint128_t) bin2;
        __uint128_t term = prod / (__uint128_t) a1;

        result += (seq_t) term;              /* exact division */
    }
    return result;
}

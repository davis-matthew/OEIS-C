/*
 * A369159  Expansion of (1/x) * Series_Reversion( x / ((1+x)^3+x^4) ).
 * https://oeis.org/A369159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..floor(n/4)} binomial(n+1,k) *
 *            binomial(3*n-3*k+3, n-4*k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 22,
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
    for (idx_t k = 0; k <= maxk; ++k)
    {
        idx_t t1 = 3 * n - 3 * k + 3;          /* 3*n - 3*k + 3 */
        idx_t t2 = n - 4 * k;                  /* n - 4*k     */
        seq_t term1 = binomial(n + 1, k);
        seq_t term2 = binomial(t1, t2);
        sum += term1 * term2;
    }
    return sum / ((seq_t)(n + 1));
}

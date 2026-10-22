/*
 * A369268  Expansion of (1/x) * Series_Reversion( x * (1-x) / (1+x^3)^3 ).
 * https://oeis.org/A369268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..floor(n/3)} binomial(3*n+3,k) *
 *            binomial(2*n-3*k, n-3*k)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 3;                     /* floor(n/3) */

    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t t1 = binomial(3 * n + 3, k);                 /* C(3n+3, k)   */
        seq_t t2 = binomial(2 * n - 3 * k, n - 3 * k);     /* C(2n-3k, n-3k) */
        sum += t1 * t2;
    }

    return sum / (n + 1);                 /* exact division */
}

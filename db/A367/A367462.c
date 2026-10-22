/*
 * A367462  Expansion of (1/x) * Series_Reversion( x / (1+x+x^3/(1+x)) ).
 * https://oeis.org/A367462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Σ_{k=0..⌊n/3⌋} binomial(n+1,k) *
 *            binomial(n-2k+1,n-3k)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 3;          /* largest k such that k ≤ n/3 */
    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t c1 = binomial((idx_t)(n + 1), k);
        seq_t c2 = binomial((idx_t)(n - 2 * k + 1), (idx_t)(n - 3 * k));
        sum += c1 * c2;
    }
    seq_t denom = (seq_t)(n + 1);
    return sum / denom;          /* exact division – result is an integer */
}

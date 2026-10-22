/*
 * A356128  a(n) = Sum_{k=1..n} k * sigma_n(k).
 * https://oeis.org/A356128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d=1..n} d^(n+1) * binomial(floor(n/d)+1, 2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t d = 1; d <= n; ++d) {
        idx_t m = n / d;                            /* floor(n/d)   */
        seq_t bin = binomial(m + 1, 2);             /* binom(m+1,2) */
        seq_t p   = big_pow((seq_t)d, (seq_t)(n + 1)); /* d^(n+1)   */
        result += p * bin;
    }
    return result;
}

/*
 * A390921  Expansion of 1/((1-x) * sqrt(1-12*x)).
 * https://oeis.org/A390921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 3^k * binomial(2*k,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)1;

    seq_t binom = (seq_t)1;   /* binom(0,0) */
    seq_t pow3  = (seq_t)1;   /* 3^0   */
    seq_t sum   = binom * pow3; /* term k=0 */

    for (idx_t k = 1; k <= n; ++k) {
        /* Update binom to binomial(2*k, k) */
        seq_t num = binom * ((seq_t)(2 * k - 1)) * ((seq_t)(2 * k));
        seq_t den = ((seq_t)k) * ((seq_t)k);
        binom = num / den;

        /* Update pow3 to 3^k */
        pow3 = pow3 * (seq_t)3;

        /* Add current term to the sum */
        sum = sum + binom * pow3;
    }

    return sum;
}

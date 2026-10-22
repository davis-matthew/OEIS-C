/*
 * A386432  a(n) = Sum_{k=0..n} n^k * binomial(n,k) * Catalan(k+1).
 * https://oeis.org/A386432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} n^k * binomial(n,k) * Catalan(k+1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum   = 0;
    seq_t n_pow = 1;   /* n^k */
    seq_t binom = 1;   /* C(n,k) */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* Catalan(k+1) */
        seq_t cat = catalan(k + 1);

        /* term = n_pow * binom * cat */
        seq_t term = n_pow;
        term *= binom;
        term *= cat;

        sum += term;

        /* Prepare values for next k */
        if (k == n) break;

        n_pow *= (seq_t)n;   /* n^(k+1) */

        /* Update binomial: C(n,k+1) = C(n,k) * (n-k)/(k+1) */
        seq_t tmp = binom;
        tmp *= (seq_t)(n - k);
        tmp /= (seq_t)(k + 1);
        binom = tmp;
    }

    return sum;
}

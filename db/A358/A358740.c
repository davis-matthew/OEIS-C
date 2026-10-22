/*
 * A358740  Expansion of Sum_{k>=0} k! * ( k * x/(1 - k*x) )^k.
 * https://oeis.org/A358740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n > 0, * a(n) = ∑_{k=1}^{n} k! * k^n *
 *            binomial(n-1, k-1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;
    seq_t fact = 1;          /* k!  */
    seq_t binom = 1;         /* binomial(n-1, k-1) */

    for (idx_t k = 1; k <= n; ++k) {
        fact *= (seq_t)k;                     /* k!  */
        seq_t power = seq_pow((seq_t)k, n);   /* k^n */
        seq_t term = fact * power;
        term *= binom;
        sum += term;

        if (k < n) {
            /* Update binom to C(n-1, k) from C(n-1, k-1) */
            /* binom_next = binom * (n - k) / k */
            binom = binom * (seq_t)(n - k);
            binom /= (seq_t)k;
        }
    }
    return sum;
}

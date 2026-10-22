/*
 * A379943  Expansion of e.g.f. 1/( exp(-x) - x )^4.
 * https://oeis.org/A379943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} (k+4)^(n-k) * binomial(k+3,3)/(n-k)!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum   = 0;
    seq_t perm  = 1;               /* perm = n!/(n-k)! for current k */
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t kk = (seq_t)k;

        /* binomial(k+3,3) = (k+3)(k+2)(k+1) / 6 */
        seq_t binom = ((kk + (seq_t)3) * (kk + (seq_t)2) *
                       (kk + (seq_t)1)) / (seq_t)6;

        /* (k+4)^(n-k) using exponentiation by squaring */
        seq_t base = kk + (seq_t)4;
        idx_t   exp = n - k;
        seq_t powv = 1;
        seq_t temp = base;
        while (exp > 0)
        {
            if (exp & 1) powv *= temp;
            temp *= temp;
            exp >>= 1;
        }

        seq_t term = perm;
        term *= powv;
        term *= binom;
        sum += term;

        if (k < n) perm *= (seq_t)(n - k);
    }
    return sum;
}

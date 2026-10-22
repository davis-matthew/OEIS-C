/*
 * A396590  Expansion of e.g.f. F_2(x)/x and F_k(x) is the k-th iteration of
 *          x*G(x) with G(x) = exp(x*G(x)^3).
 * https://oeis.org/A396590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (k+1) * (3*k+1)^(k-1) * (3*n-2*k+1)^(n-k-1)
 *            * binomial(n,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t term = binomial(n, k);
        term *= (seq_t)(k + 1);

        /* First power factor */
        if (k > 0) {
            seq_t base1 = (seq_t)(3 * k + 1);
            seq_t exp1  = (seq_t)(k - 1);          /* >= 0 */
            if (exp1 > 0)
                term *= big_pow(base1, exp1);
            /* for k==0 exp1 == -1 and base1 == 1, division by 1 does nothing */
        }

        /* Second power factor */
        seq_t base2 = (seq_t)(3 * n - 2 * k + 1);
        bool negative_exp2 = false;
        if (n > k) {
            seq_t exp2 = (seq_t)(n - k - 1);       /* >= 0 */
            if (exp2 > 0)
                term *= big_pow(base2, exp2);
        } else { /* n == k, exponent == -1 */
            negative_exp2 = true;
        }

        if (negative_exp2)
            term /= base2;                         /* exact division */

        sum += term;
    }
    return sum;
}

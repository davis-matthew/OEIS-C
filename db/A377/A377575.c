/*
 * A377575  E.g.f. satisfies A(x) = (1 + x * exp(x) * A(x))^3.
 * https://oeis.org/A377575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} k^(n-k) * binomial(3*k+3,k) /
 *            ((k+1)*(n-k)!)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   local_pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/local_pow_seq.h"

seq_t a(idx_t n)
{
    seq_t total   = 0;          /* running sum */
    seq_t fac_ratio = 1;        /* n!/(n-k)! for the current k */
    seq_t B_k = 1;              /* binomial(3*k+3,k) for the current k */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t kpow = local_pow_seq((seq_t)k, (int)(n - k));

        seq_t term = fac_ratio * B_k;
        term *= kpow;
        term /= (seq_t)(k + 1);

        total += term;

        if (k < n) {
            fac_ratio *= (n - k);   /* update n!/(n-(k+1))! */

            /* Update B_k to binomial(3*(k+1)+3,(k+1)) */
            seq_t num = (seq_t)(3 * k + 4);
            num *= (seq_t)(3 * k + 5);
            num *= (seq_t)(3 * k + 6);

            seq_t den = (seq_t)(k + 1);
            den *= (seq_t)(2 * k + 4);
            den *= (seq_t)(2 * k + 5);

            B_k = B_k * num;
            B_k /= den;
        }
    }
    return total;
}

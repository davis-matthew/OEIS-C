/*
 * A305276  Expansion of e.g.f. 1/(1 + LambertW(-x/(1 - x))).
 * https://oeis.org/A305276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \sum_{k=1}^{n} \binom{n-1}{k-1} \; k^k \; \frac{n!}{k!}
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    seq_t sum = 0;
    seq_t fact_nk = 1;               /* (n-n)! = 0! */

    for (idx_t k = n; k > 0; --k) {
        seq_t binom1 = binomial(n - 1, k - 1);   /* \binom{n-1}{k-1} */
        seq_t powk   = pow_uint((uint64_t)k, k);   /* k^k  */
        seq_t binom2 = binomial(n, k);           /* \binom{n}{k}  */

        seq_t term = binom1 * powk * binom2 * fact_nk;
        sum += term;

        if (k > 1) {
            idx_t mult = n - k + 1;            /* multiply by (n-k+1) for next iteration */
            fact_nk = fact_nk * (seq_t)mult;
        }
    }

    return sum;
}

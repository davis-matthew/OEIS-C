/*
 * A377503  E.g.f. satisfies A(x) = 1/(1 - x * exp(x) * A(x))^2.
 * https://oeis.org/A377503
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} k^(n-k) * binomial(3*k+1,k) / (
 *            (k+1)*(n-k)! )
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

seq_t a(idx_t n) {
    /* prod = n!/(n-k)! for current k, starts with k=0 (prod=1) */
    seq_t prod = 1;
    seq_t sum  = 0;

    for (idx_t k = 0; k <= n; ++k) {
        /* term:
           prod * binomial(3*k+1, k) * k^(n-k) / (k+1)  */
        /* Compute binom(3*k+1, k) */
        seq_t binom3 = binomial((idx_t)(3 * k + 1), k);

        /* Compute k^(n-k) */
        seq_t pow_term = big_pow((seq_t)k, (seq_t)(n - k));

        /* Term numerator */
        seq_t term = prod;
        term *= binom3;
        term *= pow_term;

        /* Division by (k+1) */
        term /= (seq_t)(k + 1);

        /* Add to sum */
        sum += term;

        /* Update prod for next k+1: prod_{k+1} = prod_k * (n - k) */
        if (k < n) {
            prod *= (seq_t)(n - k);
        }
    }
    return sum;
}

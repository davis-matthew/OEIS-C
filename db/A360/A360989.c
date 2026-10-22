/*
 * A360989  E.g.f. satisfies A(x) = exp(x / A(-x)^2).
 * https://oeis.org/A360989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} (-2*n + 2*k + 1)^(k-1) * (2*k)^(n-k) *
 *            binomial(n,k)
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 9, i64 n <= 15, i128 n <= 25,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* Handle the offset case explicitly */
    if (n == 0) return (seq_t)1;

    seq_t sum = 0;

    /* The k = 0 term is zero for n > 0 (contains a factor 0^(n)), so start at k = 1 */
    for (idx_t k = 1; k <= n; ++k) {
        /* Binomial coefficient binomial(n,k) */
        seq_t binom = binomial(n, k);

        /* Base1 = (-2*n + 2*k + 1) and exponent = k-1 */
        seq_t base1 = (seq_t)(-2 * (seq_t)n + 2 * (seq_t)k + 1);
        seq_t exp1  = (seq_t)(k - 1);
        seq_t pow1  = big_pow(base1, exp1);

        /* Base2 = (2*k) and exponent = n-k */
        seq_t base2 = (seq_t)(2 * (seq_t)k);
        seq_t exp2  = (seq_t)(n - k);
        seq_t pow2  = big_pow(base2, exp2);

        /* term = binom * pow1 * pow2 */
        seq_t term = binom * pow1 * pow2;

        sum += term;
    }

    return sum;
}

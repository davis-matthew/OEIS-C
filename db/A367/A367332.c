/*
 * A367332  a(n) = 27^n * Sum_{k=0..n} binomial(1/3, k)^2.
 * https://oeis.org/A367332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 27^n * Sum_{k=0..n} binomial(1/3, k)^2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute 27^n = 3^(3n) */
    idx_t exp3 = 3 * n;
    seq_t term = 1;
    for (idx_t i = 0; i < exp3; ++i) {
        term *= 3;
    }

    seq_t sum = term;

    for (idx_t k = 1; k <= n; ++k) {
        int64_t t    = (int64_t)(k - 1);
        int64_t delta = 1 - 3 * t;               /* (1 - 3(k-1)) */
        seq_t num = (seq_t)(delta * delta);      /* (1-3(k-1))^2 */
        seq_t den = (seq_t)9 * (seq_t)k * (seq_t)k; /* 9 * k^2 */

        term = term * num / den;                  /* term_k = term_{k-1} * ((1-3(k-1))^2)/(9*k^2) */
        sum += term;
    }

    return sum;
}

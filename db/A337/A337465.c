/*
 * A337465  a(n) = Sum_{k=0..n} (-n)^(n-k) * binomial(2*k,k) *
 *          binomial(2*n+1,2*k).
 * https://oeis.org/A337465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} (-n)^(n-k) * binomial(2*k,k) *
 *            binomial(2*n+1,2*k)
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 9, i64 n <= 15, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Local computation uses only seq_t, idx_t and the standard integer operations. */
    idx_t i;
    seq_t m = (seq_t)2 * (seq_t)n + 1;          /* m = 2*n + 1 */

    seq_t sum  = 0;                             /* accumulator for a(n) */
    seq_t power = 1;                            /* (-n)^(n-k), will be updated each k */
    for (i = 0; i < n; ++i) {
        power *= (seq_t)(-(long long)n);        /* initially (-n)^n */
    }

    seq_t binom_2k_k = 1;                       /* binomial(2*k,k) for current k */
    seq_t binom_2n1_2k = 1;                     /* binomial(2n+1,2k) for current k */

    for (i = 0; i <= n; ++i) {
        /* term = (-n)^(n-i) * C(2*i,i) * C(2*n+1,2*i) */
        seq_t term = power * binom_2k_k * binom_2n1_2k;
        sum += term;

        if (i == n) break;                     /* last iteration finished */

        /* Update power: divide by (-n) to reduce exponent by one */
        power /= (seq_t)(-(long long)n);

        /* Update binomial(2*(i+1),(i+1)) from current value */
        idx_t next = i + 1;
        seq_t num1 = (seq_t)(2 * next);
        seq_t num2 = (seq_t)(2 * next - 1);
        seq_t den = (seq_t)next * (seq_t)next;
        binom_2k_k = binom_2k_k * num1 * num2 / den;

        /* Update binomial(2*n+1,2*(i+1)) from current value */
        seq_t n1 = (seq_t)(m - 2 * next + 2);
        seq_t n2 = (seq_t)(m - 2 * next + 1);
        seq_t den2 = (seq_t)(2 * next) * (seq_t)(2 * next - 1);
        binom_2n1_2k = binom_2n1_2k * n1 * n2 / den2;
    }

    return sum;
}

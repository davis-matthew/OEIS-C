/*
 * A292952  E.g.f.: exp(-x * exp(x)).
 * https://oeis.org/A292952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{k=0}^{n} (-1)^k * binom(n,k) * k^{n-k}
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 15, i64 n <= 25, i128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes base^exp for seq_t using binary exponentiation */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t sum = (seq_t)0;
    seq_t binom = (seq_t)1;               /* binom(n,0) */
    for (idx_t k = 0; k <= n; ++k) {
        /* term = (-1)^k * binom * k^{n-k} */
        seq_t term_pow = pow_seq((seq_t)k, n - k);
        seq_t term = binom * term_pow;
        if (k & 1) term = -term;          /* apply (-1)^k */
        sum += term;

        /* update binom for next k+1: binom = binom * (n-k) / (k+1) */
        if (k < n) {
            seq_t next = binom * (seq_t)(n - k);
            binom = next / (seq_t)(k + 1);
        }
    }
    return sum;
}

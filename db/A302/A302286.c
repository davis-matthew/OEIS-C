/*
 * A302286  a(n) = [x^n] 1/(1 - n*x - x/(1 - n*x - x/(1 - n*x - x/(1 - n*x -
 *          x/(1 - ...))))), a continued fraction.
 * https://oeis.org/A302286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * Σ_{k=1..n} (n+1)^k * binomial(n,k) *
 *            binomial(n,k-1), with a(0) = 1. The binomial function is
 *            provided by the build utilities
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* a(0) is defined to be 1 */
    if (n == 0) return (seq_t)1;

    /* Working variables in seq_t for exact arithmetic */
    seq_t sum = 0;
    seq_t power = 1;          /* (n+1)^k, start with k=0 */
    seq_t c_prev = 1;         /* binomial(n, k-1) for k=1 starts with binomial(n,0) = 1 */

    /* n cast to seq_t once for later grouping */
    const seq_t n_seq = (seq_t)n;

    for (idx_t k = 1; k <= n; ++k) {
        /* Update (n+1)^k */
        power *= (seq_t)(n + 1);

        /* Compute binomial(n, k) from the previous binomial */
        seq_t c_k = (c_prev * (seq_t)(n - k + 1)) / (seq_t)k;

        /* Add the term to the sum */
        sum += power * c_k * c_prev;

        /* Prepare for next iteration */
        c_prev = c_k;
    }

    /* Divide the sum by n to get the final result */
    return sum / n_seq;
}

/*
 * A347154  Sum of all divisors, except the largest of every number, of the
 *          first n positive even numbers.
 * https://oeis.org/A347154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∑_{k=1}^{n} (σ(2k) – 2k) where σ(m) is the sum of all
 *            divisors of m
 * Range:     u8 n <= 15, u16 n <= 249, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate array for divisor sums up to 2*n */
    idx_t limit = 2 * n;
    seq_t *sigma = (seq_t *)malloc((limit + 1) * sizeof(seq_t));
    if (!sigma) return 0;  /* fallback – should not happen on the test platform */
    for (idx_t i = 0; i <= limit; ++i)
        sigma[i] = 0;

    /* Compute σ(m) for all m ≤ limit */
    for (idx_t d = 1; d <= limit; ++d) {
        seq_t d_seq = (seq_t)d;
        for (idx_t m = d; m <= limit; m += d) {
            sigma[m] += d_seq;
        }
    }

    /* Accumulate the required sum */
    seq_t total = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t m = 2 * k;
        total += sigma[m] - (seq_t)m;
    }

    free(sigma);
    return total;
}

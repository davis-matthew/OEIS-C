/*
 * A356043  a(n) = Sum_{k=1..n} sigma_3(k) * floor(n/k).
 * https://oeis.org/A356043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} sigma_3(k) * floor(n/k)
 * Range:     u8 n <= 5, u16 n <= 21, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the OEIS sequence A356043: a(n) = Sum_{k=1..n} sigma_3(k) * floor(n/k) */
seq_t a(idx_t n)
{
    if (n < 1) return 0;             /* undefined for n=0, but handled gracefully */

    /* allocate array for sigma_3 values, 1-indexed */
    seq_t *sigma = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!sigma) return 0;            /* allocation failure, return 0 as failure code */

    /* initialise to zero */
    for (idx_t i = 0; i <= n; ++i) sigma[i] = 0;

    /* compute sigma_3 via divisor summatory sieve */
    for (idx_t d = 1; d <= n; ++d) {
        seq_t d3 = (seq_t)d * (seq_t)d * (seq_t)d;   /* d^3 */
        for (idx_t m = d; m <= n; m += d) {
            sigma[m] += d3;
        }
    }

    /* compute the sum a(n) */
    seq_t total = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t times = (seq_t)(n / k);  /* floor(n/k) as seq_t */
        total += sigma[k] * times;
    }

    free(sigma);
    return total;
}

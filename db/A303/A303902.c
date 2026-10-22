/*
 * A303902  Expansion of (1 - x^2)*Product_{k>=2} (1 + x^k)^k.
 * https://oeis.org/A303902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = e(n) – e(n-2), where E(x) = ∏_{k=2..∞} (1 + x^k)^k (the
 *            exponential generating function of A303902), e(n) is the
 *            coefficient of x^n in E(x). For n = 0, a(0) = 1; for n = 1, a(1)
 *            = 0. The function computes all coefficients e(0)…e(n) by dynamic
 *            convolution
 * Range:     u8 n <= 12, u16 n <= 25, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* special cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    /* allocate two buffers of length n+1 (index 0..n) */
    seq_t *cur = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t *next = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!cur || !next) {
        /* out of memory – return 0, caller may check */
        if (cur) free(cur);
        if (next) free(next);
        return 0;
    }

    cur[0] = 1;  /* E(0) = 1 */

    /* iterate over k = 2 .. n */
    for (idx_t k = 2; k <= n; ++k) {
        /* zero next buffer */
        for (idx_t i = 0; i <= n; ++i) next[i] = 0;

        /* maximum j such that j*k <= n */
        idx_t max_j = (k <= n / k) ? k : n / k;

        seq_t binom = 1;  /* (k choose 0) */
        idx_t shift = 0;  /* j*k for current j */

        /* j = 0 case: shift = 0, binom = 1 */
        for (idx_t i = 0; i <= n; ++i)
            next[i] += cur[i];   /* next[i] += cur[i] * 1 */

        for (idx_t j = 1; j <= max_j; ++j) {
            /* update binomial coefficient */
            binom = binom * (seq_t)(k - j + 1) / (seq_t)j;
            shift = j * k;

            /* convolve cur with x^{shift} * binom */
            for (idx_t i = 0; i + shift <= n; ++i) {
                /* cur[i] * binom may be large; addition truncates as per seq_t */
                next[i + shift] += cur[i] * binom;
            }
        }

        /* swap buffers for next iteration */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    /* a(n) = cur[n] - cur[n-2]  (cur holds e(n)) */
    seq_t result = cur[n] - cur[n - 2];

    /* free temporary buffers */
    free(cur);
    free(next);

    return result;
}

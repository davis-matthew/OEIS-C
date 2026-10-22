/*
 * A319218  Expansion of e.g.f. Product_{k>=1} (1 - x^k/(k - 1)!).
 * https://oeis.org/A319218
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 14, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes the coefficients of the exponential generating
 * function product_{k>=1} (1 - x^k/(k-1)!).
 *
 * Recurrence used (derived from multiplying the product factor by factor):
 *   a(0) = 1;
 *   For k = 1 .. n:
 *     For i = n .. k descending (here we use an auxiliary array):
 *        a_new[i] = a_old[i] - binom(i, k) * k * a_old[i-k]
 *
 * After processing all k, a_new[n] is the required a(n).
 */

#include <stddef.h>
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    size_t N = (size_t)n;          /* memory size safe for n <= 10000 */

    seq_t *g  = alloc_seq(N + 1);   /* coefficients from previous k */
    seq_t *ng = alloc_seq(N + 1);   /* new coefficients for current k */

    /* initialise: only constant term is 1 */
    for (size_t i = 0; i <= N; ++i) g[i] = (seq_t)0;
    g[0] = (seq_t)1;

    for (size_t k = 1; k <= N; ++k) {
        /* coefficients below k stay unchanged */
        for (size_t i = 0; i < k; ++i) ng[i] = g[i];

        /* binomial coefficient binom(i, k) for current k, updated incrementally */
        seq_t binom = (seq_t)1;               /* binom(k, k) = 1 */

        for (size_t i = k; i <= N; ++i) {
            if (i > k) {
                /* binom(i, k) = binom(i-1, k) * i / (i-k)  (exact division) */
                binom = (binom * (seq_t)i) / (seq_t)(i - k);
            }

            /* term to subtract: k * binom(i, k) * a[i-k] (previous values) */
            seq_t term = g[i - k];
            term *= binom;
            term *= (seq_t)k;

            ng[i] = g[i] - term;
        }

        /* swap arrays for next iteration */
        seq_t *tmp = g;
        g = ng;
        ng = tmp;
    }

    seq_t result = g[N];
    free_seq(g);
    free_seq(ng);
    return result;
}

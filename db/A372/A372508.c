/*
 * A372508  Expansion of e.g.f. exp(sinh(x)) - exp(cosh(x) - 1).
 * https://oeis.org/A372508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A372508: number of partitions of an n‑set into odd blocks minus
 * number of partitions into even blocks.
 *
 * The counts f[n] (odd) and g[n] (even) satisfy
 *
 *     f(0) = g(0) = 1
 *     f(n) = Σ_{k odd}  C(n-1, k-1) * f(n-k)
 *     g(n) = Σ_{k even} C(n-1, k-1) * g(n-k)
 *
 * for n ≥ 1.  The desired term is a(n) = f(n) – g(n).
 *    (Indices start at 0.)
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static arrays holding computed f[n] and g[n] */
    static seq_t *f = NULL;
    static seq_t *g = NULL;
    static idx_t max_n = 0;

    /* initialise if first call */
    if (f == NULL) {
        f = (seq_t *)malloc(sizeof(seq_t));
        g = (seq_t *)malloc(sizeof(seq_t));
        f[0] = 1;
        g[0] = 1;
        max_n = 0;
    }

    /* grow arrays if a larger n is requested */
    if (n > max_n) {
        idx_t new_size = n + 1;
        f = (seq_t *)realloc(f, new_size * sizeof(seq_t));
        g = (seq_t *)realloc(g, new_size * sizeof(seq_t));

        /* compute new values up to n */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            seq_t sum_f = 0;
            seq_t sum_g = 0;
            /* binom = C(i-1, k-1) */
            seq_t binom = 1;

            for (idx_t k = 1; k <= i; ++k) {
                if (k & 1)          /* odd k */
                    sum_f += binom * f[i - k];
                else                /* even k */
                    sum_g += binom * g[i - k];

                /* update binom for the next k */
                if (k < i) {
                    seq_t num = (seq_t)(i - k);   /* i - k = (i-1) - (k-1) */
                    seq_t den = (seq_t)k;
                    binom = (binom * num) / den;   /* exact division */
                }
            }
            f[i] = sum_f;
            g[i] = sum_g;
        }
        max_n = n;
    }

    return f[n] - g[n];
}

/*
 * A396891  Convolution of A001157 and A000219.
 * https://oeis.org/A396891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * p(n), where p(n) is the plane‑partition number
 *            A000219 (coefficients of ∏_{k≥1} (1−x^k)^{−k}). The recurrence
 *            for p(n) is n·p(n) = Σ_{k=1}^{n} σ₂(k)·p(n−k), where
 *            σ₂(k)=∑_{d|k} d². We compute σ₂(k) by a divisor sieve and p(n)
 *            by the above recurrence. a(0)=0 is handled explicitly. All r...
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state kept between calls                                    */
    static seq_t *sigma = NULL; /* σ₂ values                                    */
    static seq_t *p     = NULL; /* plane‑partition numbers                  */
    static size_t  size  = 0;   /* allocated length                           */
    static idx_t   computed = 0;/* largest n fully processed                 */

    if (n == 0) {                 /* a(0) = 0                                         */
        return 0;
    }

    /* Allocate / grow arrays if necessary                                 */
    if (n + 1 > size) {
        size_t new_size = (size_t)n + 1;
        sigma = (seq_t *)realloc(sigma, new_size * sizeof(seq_t));
        p     = (seq_t *)realloc(p,     new_size * sizeof(seq_t));
        /* initialise new entries                                         */
        for (size_t i = size; i < new_size; ++i) {
            sigma[i] = 0;
            p[i]     = 0;
        }
        p[0] = 1;                 /* p(0) = 1                                        */
        size = new_size;
    }

    /* Extend precomputed data up to n                                     */
    if (n > computed) {
        /* 1. Compute σ₂(k) for k = computed+1 … n                         */
        for (idx_t d = 1; d <= n; ++d) {
            seq_t d2 = (seq_t)d * (seq_t)d;
            idx_t start = (computed / d + 1ULL) * d;
            for (idx_t m = start; m <= n; m += d) {
                sigma[(size_t)m] += d2;
            }
        }

        /* 2. Compute p(k) for k = computed+1 … n                           */
        for (idx_t i = computed + 1; i <= n; ++i) {
            seq_t sum = 0;
            for (idx_t k = 1; k <= i; ++k) {
                sum += sigma[(size_t)k] * p[(size_t)(i - k)];
            }
            p[(size_t)i] = sum / (seq_t)i;      /* exact division               */
        }

        computed = n;
    }

    /* Return a(n) = n * p(n)                                             */
    return (seq_t)n * p[(size_t)n];
}

/*
 * A354546  Expansion of e.g.f. exp(x)^( cos(x) - sin(x) ).
 * https://oeis.org/A354546
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1 ; a(n) = Σ_{k=1..n} (-1)^⌊k/2⌋ · k · binomial(n‑1,k‑1)
 *            · a(n‑k)
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 14, i64 n <= 23, i128 n <= 23,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* static cache – cache[0..cached] holds a(0) … a(cached) */
    static seq_t *cache = NULL;
    static idx_t cached = 0U;

    /* initialise cache for a(0) */
    if (cache == NULL) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) return 0;          /* out‑of‑memory: fail silently */
        cache[0] = 1;                  /* a(0) */
        cached = 0U;
    }

    /* grow cache up to n and compute missing entries */
    if (n > cached) {
        size_t newSize = (size_t)(n + 1U);
        seq_t *tmp = (seq_t*)realloc(cache, newSize * sizeof(seq_t));
        if (!tmp) return 0;            /* out‑of‑memory: fail silently */
        cache = tmp;

        for (idx_t i = cached + 1; i <= n; ++i) {
            seq_t sum = 0;
            for (idx_t k = 1; k <= i; ++k) {
                seq_t term = cache[i - k];
                term *= (seq_t)k;
                term *= binomial(i - 1, k - 1);

                /* (-1)^floor(k/2): parity of k>>1 */
                if ((k >> 1) & 1)   sum -= term;
                else                sum += term;
            }
            cache[i] = sum;
        }
        cached = n;
    }

    return cache[n];
}

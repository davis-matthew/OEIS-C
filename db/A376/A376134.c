/*
 * A376134  a(0) = 1; a(n) = Sum_{k=0..n-1} (-1)^k * (k+1) * a(k) * a(n-k-1).
 * https://oeis.org/A376134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1; a(n) = Σ_{k=0}^{n-1} (-1)^k * (k+1) * a(k) * a(n-1-k)
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation caches computed values for arbitrary‑order calls. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;   /* allocated capacity */
    static idx_t last_calc   = 0;   /* highest n already computed */

    /* initialise cache with a(0) = 1 */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failure, return 0 */
        cache[0] = (seq_t)1;
        last_calc = 0;
    }

    /* grow cache if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) {
            new_cap = (new_cap == 0) ? 1 : new_cap * 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* allocation failure, return 0 */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute any missing terms up to n */
    if (n > last_calc) {
        for (idx_t i = last_calc + 1; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            for (idx_t k = 0; k < i; ++k) {
                seq_t prod = cache[k] * cache[i - 1 - k];
                prod *= (seq_t)(k + 1);
                if (k & (idx_t)1) prod = -prod;   /* apply (-1)^k */
                sum += prod;
            }
            cache[i] = sum;
        }
        last_calc = n;
    }

    return cache[n];
}

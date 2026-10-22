/*
 * A360086  a(n) = Sum_{k=0..n} (-1)^k * binomial(5*k,n-k) * Catalan(k).
 * https://oeis.org/A360086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = \sum_{k=0}^n (-1)^k * binomial(5*k, n-k) * Catalan(k)
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 16, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   binomial, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/catalan.h"   /* provides: seq_t catalan(idx_t k) */
#include "../utils/binomial.h"  /* provides: seq_t binomial(idx_t a, idx_t b) */

/* seq_t and idx_t are defined by the build system.  */
seq_t a(idx_t n)
{
    /* Memoisation cache.  */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;      /* number of entries already computed   */
    static size_t cache_cap = 0;      /* allocated capacity of cache          */

    /* Expand cache if necessary.  */
    if ((size_t)(n + 1) > cache_cap) {
        if (cache_cap == 0) cache_cap = 1;
        while (cache_cap < (size_t)(n + 1)) cache_cap <<= 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* Compute all values from cache_size up to n.  */
    for (idx_t m = cache_size; m <= n; ++m) {
        seq_t sum = (seq_t)0;

        for (idx_t k = 0; k <= m; ++k) {
            /* Compute Catalan(k) once.  */
            seq_t cat = catalan(k);

            /* binomial(5*k, m-k) : a = 5*k, b = m-k  */
            seq_t bin = binomial(5 * k, m - k);

            /* term = (-1)^k * bin * cat  */
            seq_t term = cat * bin;
            if (k & 1U) {          /* odd k => multiply by -1 */
                term = -term;
            }

            sum += term;
        }

        cache[m] = sum;
    }

    cache_size = n + 1;
    return cache[n];
}

/*
 * A400314  G.f. A(x) satisfies A(x) = A(x^3/(1 - x)) / (1 - x)^2, with A(0) =
 *          1.
 * https://oeis.org/A400314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/3⌋} binomial(n-2*k+1, k+1) * a(k) with a(0)
 *            = 1
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* C11 / C++17 compatible implementation using a static dynamic cache. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* ---------- static cache ---------- */
    static seq_t *cache     = NULL;   /* array of computed values   */
    static size_t cache_sz  = 0;      /* current allocated size    */
    static idx_t  cache_max = (idx_t)-1; /* highest index already known */

    /* first call: initialise cache[0] = 1 */
    if (cache == NULL) {
        cache_sz  = 1;
        cache     = (seq_t *)malloc(sizeof(seq_t) * cache_sz);
        if (cache == NULL) abort();     /* allocation failure */
        cache[0]  = (seq_t)1;
        cache_max = 0;
    }

    /* grow the cache if the requested index is beyond the current size */
    if ((size_t)n >= cache_sz) {
        size_t new_sz = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (tmp == NULL) abort();       /* allocation failure */
        cache = tmp;
        cache_sz = new_sz;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t sum = 0;
        idx_t max_k = i / 3;
        for (idx_t k = 0; k <= max_k; ++k) {
            seq_t b = binomial(i - 2 * k + 1, k + 1);
            sum += b * cache[k];
        }
        cache[i] = sum;
    }

    cache_max = n;
    return cache[n];
}

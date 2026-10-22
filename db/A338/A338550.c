/*
 * A338550  Number of binary trees of height n such that the number of nodes
 *          at depth d equals d+1 for every d = 0..n.
 * https://oeis.org/A338550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(n) = binomial(2*n, n+1) * a(n-1) for n >= 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache = NULL;      /* array of terms  */
    static idx_t cached_max = 0;     /* largest n already computed */

    /* initialise cache with a(0) */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));        /* cast for C++ */
        if (cache == NULL) abort();
        cache[0] = 1;                                 /* a(0) = 1 */
        cached_max = 0;
    }

    /* if the requested term is already cached, return it */
    if (n <= cached_max) return cache[n];

    /* expand cache to accommodate up to index n */
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t)); /* cast for C++ */
    if (tmp == NULL) abort();
    cache = tmp;

    /* compute terms for indices (cached_max+1) .. n */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        seq_t bin = binomial((idx_t)(2 * i), (idx_t)(i + 1));
        cache[i] = cache[i - 1] * bin;   /* recurrence */
    }

    cached_max = n;
    return cache[n];
}

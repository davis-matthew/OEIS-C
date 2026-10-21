/*
 * A297445  a(n) = a(n-1) + 9*a(n-2) - 9*a(n-3), where a(0) = 1, a(1) = 5,
 *          a(2) = 11.
 * https://oeis.org/A297445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 9*a(n-2) - 9*a(n-3), a(0)=1, a(1)=5, a(2)=11
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache     = NULL;   /* cached values */
static size_t cache_cap = 0;      /* capacity of 'cache' array */
static idx_t cache_last = 0;      /* index of the last computed element */

/* Compute the n‑th term of OEIS A297445 */
seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 3;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;          /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)11;
        cache_last = 2;
    }

    /* value already computed */
    if (n <= cache_last)
        return cache[n];

    /* grow the cache if necessary */
    if ((size_t)(n) + 1 > cache_cap) {
        size_t new_cap = (size_t)(n) + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;          /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms iteratively */
    seq_t nine = (seq_t)9;
    for (idx_t i = cache_last + 1; i <= n; ++i)
        cache[i] = cache[i-1] + nine * cache[i-2] - nine * cache[i-3];

    cache_last = n;
    return cache[n];
}

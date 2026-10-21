/*
 * A246313  G.f.: (-1+6*x)/(1-3*x-2*x^2).
 * https://oeis.org/A246313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3*a(n-1) + 2*a(n-2), a(0) = -1, a(1) = 3
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 17, i64 n <= 34, i128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* stored sequence values      */
static idx_t cache_cap = 0;          /* allocated array size (bytes) */
static idx_t cache_len = 0;          /* number of values already filled */

/* ensure that the cache can hold indices up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_cap)
        return;
    idx_t new_cap = cache_cap > 0 ? cache_cap : 2;
    while (new_cap <= n)
        new_cap <<= 1;               /* double until big enough   */
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
    if (!tmp)
        abort();                     /* allocation failed          */
    cache = tmp;
    cache_cap = new_cap;
}

/* compute the nth term of A246313 */
seq_t a(idx_t n)
{
    /* lazy allocation of cache */
    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache)
            abort();
        cache_len = 0;
    }

    /* the sequence starts with -1, 3; initialise these once */
    if (cache_len == 0) {
        cache[0] = -1;
        cache[1] = 3;
        cache_len = 2;
    }

    /* if the requested term is already computed, return it */
    if (n < cache_len)
        return cache[n];

    ensure_cache(n);

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = 3 * cache[i - 1] + 2 * cache[i - 2];
    }
    cache_len = n + 1;

    return cache[n];
}

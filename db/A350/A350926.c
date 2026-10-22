/*
 * A350926  a(0) = 1, a(1) = 17, and a(n) = 23*a(n-1) - a(n-2) - 4 for n >= 2.
 * https://oeis.org/A350926
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 17, a(n) = 23*a(n-1) - a(n-2) - 4 for n >= 2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* dynamically grown cache of computed terms */
static idx_t  cache_size = 0; /* number of entries in cache */

/* Ensure that cache contains values up to index n. */
static void
ensure_cache(idx_t n)
{
    if (cache_size > n)
        return;                 /* already computed */

    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (tmp == NULL)
        exit(1);                /* allocation failure */
    cache = tmp;

    /* Initialise new entries if cache was empty. */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;
        if (new_size > 1)
            cache[1] = (seq_t)17;
    } else if (cache_size == 1) {
        /* cache[0] already exists; set cache[1] if it is new */
        if (new_size > 1)
            cache[1] = (seq_t)17;
    }

    /* Compute remaining terms, starting from current cache_size. */
    for (idx_t i = cache_size; i < new_size; ++i) {
        if (i == 0) continue;
        if (i == 1) continue;
        cache[i] = (seq_t)(23 * cache[i-1] - cache[i-2] - 4);
    }

    cache_size = new_size;
}

seq_t
a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}

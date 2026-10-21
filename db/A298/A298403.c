/*
 * A298403  a(n) = 2*a(n-1) - a(n-3) + a(floor(n/2)), where a(0) = 1, a(1) =
 *          2, a(2) = 3.
 * https://oeis.org/A298403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) – a(n-3) + a(floor(n/2)),
   with a(0)=1, a(1)=2, a(2)=3.  The sequence is memoized in a
   static array so that callers may request terms in any order. */
#include <stdlib.h>
#include <stddef.h>
/* static cache of computed terms */
static seq_t *cache = NULL;     /* array of values */
static size_t cache_cap = 0;     /* allocated capacity of cache array    */
static idx_t cache_size = 0;     /* number of valid entries in cache     */

/* ensure the cache has at least <needed> entries */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap? (cache_cap << 1) : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *newc = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!newc)
        abort(); /* out of memory */
    if (cache) {
        for (size_t i = 0; i < cache_size; ++i)
            newc[i] = cache[i];
        free(cache);
    }
    cache = newc;
    cache_cap = new_cap;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* initialise cache with the first three known terms */
    if (cache_size < 3) {
        ensure_cache_cap(3);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)3;
        cache_size = 3;
    }

    /* extend the cache up to n */
    if ((idx_t)cache_size <= n) {
        ensure_cache_cap((size_t)n + 1);
        for (idx_t i = cache_size; i <= n; ++i) {
            /* i >= 3 because cache_size is at least 3 */
            seq_t term = (seq_t)2 * cache[i - 1]
                         - cache[i - 3]
                         + cache[i / 2];
            cache[i] = term;
        }
        cache_size = n + 1;
    }

    return cache[n];
}

/*
 * A285665  a(n) = a(n-1) + 2*a(n-2) - a(n-3) - a(n-4) + 2*a(n-5) - 2*a(n-7)
 *          for n>7 and where a(0)=2, a(1)=3, a(2)=6, a(3)=10, a(4)=17,
 *          a(5)=29, a(6)=51.
 * https://oeis.org/A285665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2) - a(n-3) - a(n-4) + 2*a(n-5) - 2*a(n-7)
 *            (n > 7) * initial values: a(0)=2, a(1)=3, a(2)=6, a(3)=10,
 *            a(4)=17, a(5)=29, a(6)=51, a(7)=90
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 37, u64 n <= 76, u128 n <= 154,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

static seq_t *cache = NULL;      /* array of computed values      */
static size_t cache_cap = 0;     /* current capacity of cache      */
static idx_t last_computed = (idx_t)-1;  /* highest index that has been computed */

/* initialise the cache with the first 8 terms */
static void init_cache(void)
{
    ensure_cache(7, &cache, &cache_cap);
    cache[0] = (seq_t)2;
    cache[1] = (seq_t)3;
    cache[2] = (seq_t)6;
    cache[3] = (seq_t)10;
    cache[4] = (seq_t)17;
    cache[5] = (seq_t)29;
    cache[6] = (seq_t)51;
    cache[7] = (seq_t)90;
    last_computed = 7;
}

seq_t a(idx_t n)
{
    /* initialise lazily */
    if (cache == NULL || last_computed == (idx_t)-1)
        init_cache();

    /* already known */
    if (n <= last_computed)
        return cache[n];

    /* extend the cache to n and compute missing terms */
    ensure_cache(n, &cache, &cache_cap);
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* recurrence applies for i >= 8; i-7 is valid because
           last_computed >= 7 after init_cache() */
        cache[i] = cache[i-1]
                 + (seq_t)2 * cache[i-2]
                 - cache[i-3]
                 - cache[i-4]
                 + (seq_t)2 * cache[i-5]
                 - (seq_t)2 * cache[i-7];
    }
    last_computed = n;
    return cache[n];
}

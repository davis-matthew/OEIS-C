/*
 * A314658  Coordination sequence Gal.5.14.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) - a(n-3) + a(n-4) + a(n-6) - a(n-7) + a(n-9) -
 *            a(n-10), with initial values a(0)=1, a(1)=5, a(2)=9, a(3)=13,
 *            a(4)=15, a(5)=22, a(6)=26, a(7)=34, a(8)=36, a(9)=41, a(10)=41,
 *            a(11)=49, a(12)=53, a(13)=62
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;    /* array of already computed values */
static size_t cache_len = 0;   /* number of values stored so far */
static size_t cache_cap = 0;   /* allocated capacity */

/* Base values for n = 0 .. 13 */
static const seq_t base_values[14] = {
    (seq_t)1,  (seq_t)5,  (seq_t)9,  (seq_t)13,
    (seq_t)15, (seq_t)22, (seq_t)26, (seq_t)34,
    (seq_t)36, (seq_t)41, (seq_t)41, (seq_t)49,
    (seq_t)53, (seq_t)62
};

/* Forward declarations of helper functions */
static void init_cache(void);
static void grow_cache(size_t required);

/* Initialise the cache with base values if it hasn't been done yet */
static void init_cache(void)
{
    if (cache != NULL)
        return;                /* already initialised */

    cache_cap = 32;            /* start with a modest capacity */
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (cache == NULL)
        return;                /* out of memory – not expected in this context */

    for (size_t i = 0; i < 14; ++i)
        cache[i] = base_values[i];

    cache_len = 14;            /* we have values for 0..13 */
}

/* Ensure the cache can hold at least 'required' elements */
static void grow_cache(size_t required)
{
    if (required <= cache_cap)
        return;                /* already enough */

    size_t new_cap = cache_cap ? cache_cap : 32;
    while (new_cap <= required)
        new_cap <<= 1;         /* double until we exceed 'required' */

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL)
        return;                /* out of memory – not expected */

    cache = tmp;
    cache_cap = new_cap;
}

/* The required sequence function */
seq_t a(idx_t n)
{
    if (n <= 13)                /* trivial case using the base values */
        return base_values[(size_t)n];

    init_cache();               /* make sure the cache exists */

    /* Extend the cache if we haven't computed up to n yet */
    if ((size_t)n >= cache_len) {
        grow_cache((size_t)n + 1);
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            cache[i] = cache[i - 1]
                     - cache[i - 3]
                     + cache[i - 4]
                     + cache[i - 6]
                     - cache[i - 7]
                     + cache[i - 9]
                     - cache[i - 10];
        }
        cache_len = (size_t)n + 1;   /* now we have values up to n */
    }

    return cache[(size_t)n];
}

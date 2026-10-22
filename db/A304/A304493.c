/*
 * A304493  a(0) = 1; a(n) = a(n-a(floor(n/2))) + a(n-a(floor(n/4))).
 * https://oeis.org/A304493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(0) = 1 a(n) = a(n - a(floor(n/2))) + a(n - a(floor(n/4))) for
 *            n > 0 Sequence variant: Hofstadter‑style recurrence. The
 *            implementation memoises results in a static array to allow
 *            multiple, non‑sequential calls
 * Range:     u8 n <= 165, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */
#include <stddef.h>   /* NULL */

/* static cache for computed values */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* ensure the cache has room for indices up to n (inclusive) */
static void ensure_cache_size(idx_t n)
{
    if (cache == NULL) {
        cache_size = n + 1;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL)
            return;                      /* out of memory – return NULL pointer */
        /* initialise the base value a(0) = 1, remaining entries = 0 */
        cache[0] = (seq_t)1;
        for (idx_t i = 1; i < cache_size; ++i)
            cache[i] = (seq_t)0;
    }
    else if (cache_size <= n) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL)
            return;                      /* out of memory – keep old cache */
        /* initialise new entries to 0 */
        for (idx_t i = cache_size; i < new_size; ++i)
            tmp[i] = (seq_t)0;
        cache      = tmp;
        cache_size = new_size;
    }
}

/* recursive computation with memoisation */
seq_t a(idx_t n)
{
    ensure_cache_size(n);

    /* cached value?  All sequence terms are positive, so 0 is unused. */
    if (cache[n] != (seq_t)0)
        return cache[n];

    /* compute recursively: a(n) = a(n−a(n/2)) + a(n−a(n/4)) */
    idx_t k1 = n >> 1;          /* floor(n/2)   */
    idx_t k2 = n >> 2;          /* floor(n/4)   */
    idx_t n1 = n - (idx_t)a(k1);
    idx_t n2 = n - (idx_t)a(k2);

    seq_t val = a(n1) + a(n2);
    cache[n]  = val;
    return val;
}

/*
 * A332060  a(n) = 3*a(n-1) + a(n-2) after initial values a(0..5) = (0, 1, 2,
 *          3, 5, 13).
 * https://oeis.org/A332060
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 21, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0..5) = 0,1,2,3,5,13
   a(n)   = 3*a(n-1) + a(n-2)   for n >= 6
   computed lazily with memoisation */
#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;   /* memoisation table */
static size_t cache_cap = 0;
static idx_t last_computed = 5;      /* highest index that has a value stored */

/* Initialise cache with the first six terms. */
static void init_cache(void)
{
    if (cache != NULL) return;
    cache_cap = 6;
    cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    cache[0] = (seq_t)0;
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)2;
    cache[3] = (seq_t)3;
    cache[4] = (seq_t)5;
    cache[5] = (seq_t)13;
    last_computed = 5;
}

/* Ensure cache has room for indices up to n.  The table is only grown,
   never shrunk. */
static void ensure_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 8;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* First call initialises the cache. */
    if (cache == NULL) init_cache();

    /* If the requested index is within the cached range, return immediately. */
    if (n <= last_computed) return cache[n];

    /* Grow the cache to hold the new indices. */
    ensure_capacity(n);

    /* Compute missing values using the recurrence. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        cache[i] = 3 * cache[i-1] + cache[i-2];
    }
    last_computed = n;
    return cache[n];
}

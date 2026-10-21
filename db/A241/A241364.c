/*
 * A241364  Number of n X 2 0..2 arrays with no element equal to fewer
 *          vertical neighbors than horizontal neighbors, with new values 0..2
 *          introduced in row major order.
 * https://oeis.org/A241364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) + 2*a(n-3) - 8*a(n-4) for n>5
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache of previously computed terms.
   cache[i] holds a(i+1). */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* Number of values already stored. */
static size_t cache_cap = 0;   /* Allocated space for cache. */

/* Ensure the cache has space for at least 'needed' elements. */
static void ensure_capacity(size_t needed) {
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 1;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();               /* Allocation failed: terminate. */
    cache = tmp;
    cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* Convert idx_t to size_t for array indexing (n is always ≥1 for the
       allowed range of the function). */
    size_t target = (size_t)n;

    /* Initialise the first five values on the first call. */
    if (cache_len < 5) {
        ensure_capacity(5);
        cache[0] = (seq_t)1;      /* a(1) */
        cache[1] = (seq_t)7;      /* a(2) */
        cache[2] = (seq_t)47;     /* a(3) */
        cache[3] = (seq_t)326;    /* a(4) */
        cache[4] = (seq_t)2284;   /* a(5) */
        cache_len = 5;
    }

    /* If the requested value is already cached, return it. */
    if (target <= cache_len)
        return cache[target - 1];

    /* Expand the cache up to the required index. */
    ensure_capacity(target);

    /* Compute terms from cache_len up to target-1 using the recurrence. */
    for (size_t i = cache_len; i < target; ++i) {
        /* i corresponds to a(i+1). */
        seq_t term = 7 * cache[i - 1];
        term += 2 * cache[i - 3];
        term -= 8 * cache[i - 4];
        cache[i] = term;
    }
    cache_len = target;

    return cache[target - 1];
}

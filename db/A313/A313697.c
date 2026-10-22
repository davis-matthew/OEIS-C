/*
 * A313697  Coordination sequence Gal.5.202.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A313697: Coordination sequence Gal.5.202.4
   Formula: a(n) = 2*a(n-5) - a(n-10)   for n > 17
   Initial values (a(0) … a(17)):
   1, 5, 10, 15, 19, 24, 30, 36, 40, 45,
   50, 54, 63, 66, 69, 76, 78, 89 */

#include <stdlib.h>

static const seq_t init_vals[18] = {
    1, 5, 10, 15, 19, 24, 30, 36, 40, 45,
    50, 54, 63, 66, 69, 76, 78, 89
};

static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t cache_len = 0;

/* Make sure the cache can hold at least 'needed' entries. */
static void ensure_cache(idx_t needed)
{
    if (cache == NULL) {
        /* allocate at least the needed capacity */
        idx_t cap = needed;
        if (cap < 18) cap = 18;
#ifdef __cplusplus
        cache = new seq_t[cap];
#else
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
#endif
        cache_cap = cap;
    } else if (cache_cap < needed) {
        /* enlarge the cache */
        idx_t new_cap = needed;
        seq_t *newptr;
#ifdef __cplusplus
        newptr = new seq_t[new_cap];
        /* copy previous contents */
        for (idx_t j = 0; j < cache_cap; ++j) newptr[j] = cache[j];
        delete[] cache;
#else
        newptr = realloc(cache, new_cap * sizeof(seq_t));
#endif
        cache = newptr;
        cache_cap = new_cap;
    }
    /* If cache exists and is already big enough, nothing to do. */
}

/* Main function compute a(n). */
seq_t a(idx_t n)
{
    /* Ensure the cache has room for the requested index. */
    if (n >= cache_cap) {
        ensure_cache(n + 1);
    }

    /* If the cache has never been initialized, set the first 18 values. */
    if (cache_len < 18) {
        for (idx_t i = 0; i < 18; ++i) {
            cache[i] = init_vals[i];
        }
        cache_len = 18;
    }

    /* Compute any missing values up to n. */
    if (n >= cache_len) {
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i < 18) {
                cache[i] = init_vals[i];
            } else {
                cache[i] = (seq_t)2 * cache[i - 5] - cache[i - 10];
            }
        }
        cache_len = n + 1;
    }

    return cache[n];
}

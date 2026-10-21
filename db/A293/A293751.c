/*
 * A293751  Values of positive integer i such that floor(tan(-i)) = 1.
 * https://oeis.org/A293751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 1040, u32 n <= 1040, u64 n <= 1040,
 *            u128 n <= 1040, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A293751: integers i such that floor(tan(-i)) == 1.
   The function incrementally scans integers, storing each hit
   in a static cache so that subsequent calls need only
   extend the cache. */

#include <stdlib.h>
#include <math.h>

static seq_t *cache = NULL;       /* cached sequence values */
static size_t cache_cap = 0;      /* allocated capacity */
static size_t cache_len = 0;      /* number of values stored */

static idx_t next_i = 1;          /* next integer to test */
static idx_t seen = 0;            /* number of values found so far */

/* Ensure the cache has enough space to store one more value */
static void grow_cache(void) {
    size_t new_cap = cache_cap ? cache_cap << 1U : 8U;
    seq_t *new_p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_p;
    cache_cap = new_cap;
}

seq_t a(idx_t n) {
    if (n == 0ULL) return 0ULL;   /* offset starts at 1, safety check */

    /* Extend the cache until we have at least n elements */
    while (cache_len < n) {
        double t = tan(- (double)next_i);
        double f = floor(t);
        if (f == 1.0) {
            if (cache_len == cache_cap) grow_cache();
            cache[cache_len] = (seq_t)next_i;
            cache_len++;
        }
        next_i++;
    }

    return cache[n - 1];
}

/*
 * A387779  Upper (1/2)-midsequence of (F(2n)) and (F(2n+1)), where F=A000045
 *          (Fibonacci numbers); see Comments.
 * https://oeis.org/A387779
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil((F(2*n) + F(2*n+1))/2)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3*a(n-1) - a(n-2) + a(n-3) - 3*a(n-4) + a(n-5) */
/* Initial values: a(0)=1, a(1)=2, a(2)=4, a(3)=11, a(4)=28 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cache of computed a(n) values */
static idx_t cache_size = 0;     /* number of elements stored */

/* Helper: initialize cache with first five terms */
static void init_cache(void) {
    if (cache == NULL) {
        cache = (seq_t *)malloc(5 * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)11;
        cache[4] = (seq_t)28;
        cache_size = 5;
    }
}

/* Ensure that cache has space for at least index n inclusive */
static void ensure_cache(idx_t n) {
    if (cache_size > n) return;
    idx_t new_size = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) {
        /* If realloc fails, abort to avoid undefined behaviour */
        abort();
    }
    cache = new_cache;
    /* Compute missing entries using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* a(i) = 3*a(i-1) - a(i-2) + a(i-3) - 3*a(i-4) + a(i-5) */
        cache[i] = 3 * cache[i - 1]
                   - cache[i - 2]
                   + cache[i - 3]
                   - 3 * cache[i - 4]
                   + cache[i - 5];
    }
    cache_size = new_size;
}

seq_t a(idx_t n) {
    init_cache();
    ensure_cache(n);
    return cache[n];
}

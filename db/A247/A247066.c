/*
 * A247066  Dynamic Betting Game D(n,6,2).
 * https://oeis.org/A247066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-11) - a(n-12) Initial values (with a(0)=0
 *            for convenience): a(1)=1, a(2)=2, a(3)=6, a(4)=8, a(5)=12,
 *            a(6)=16, a(7)=17, a(8)=21, a(9)=24, a(10)=27, a(11)=32, a(12)=33
 * Range:     u8 n <= 87, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;      /* cache[0] is a(0) (=0) */
static size_t cache_cap = 0;     /* allocated capacity (>= last_computed+1) */
static idx_t  last_computed = 0; /* index of greatest term computed so far */

/* Ensure the cache has space for indices up to `max_needed` and that those
   entries are computed. */
static void ensure_cache(idx_t max_needed) {
    /* Initial allocation if first call */
    if (cache == NULL) {
        size_t init = max_needed + 1;
        if (init < 13) init = 13;               /* need at least indices 0..12 */
        cache = (seq_t *)malloc(init * sizeof(seq_t));
        cache_cap = init;

        /* Set initial values */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;  /* a(1) */
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)6;
        cache[4] = (seq_t)8;
        cache[5] = (seq_t)12;
        cache[6] = (seq_t)16;
        cache[7] = (seq_t)17;
        cache[8] = (seq_t)21;
        cache[9] = (seq_t)24;
        cache[10] = (seq_t)27;
        cache[11] = (seq_t)32;
        cache[12] = (seq_t)33;
        last_computed = 12;
    }

    /* Grow the buffer if needed */
    if ((size_t)(max_needed + 1) > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)(max_needed + 1) > new_cap) new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute missing entries using the recurrence */
    if (max_needed > last_computed) {
        for (idx_t i = last_computed + 1; i <= max_needed; ++i) {
            /* a(i) = a(i-1) + a(i-11) - a(i-12) */
            cache[i] = cache[i - 1] + cache[i - 11] - cache[i - 12];
        }
        last_computed = max_needed;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* a(0) is defined as 0, though not used */
    ensure_cache(n);
    return cache[n];
}

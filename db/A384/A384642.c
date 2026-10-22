/*
 * A384642  Expansion of (1+x-x^2+x^3) / (1-x-5*x^2+x^3+2*x^4).
 * https://oeis.org/A384642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   a(n) = a(n-1) + 5*a(n-2) - a(n-3) - 2*a(n-4) for n >= 4 with
 *            a(0)=1, a(1)=2, a(2)=6, a(3)=16
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Initial values for the first four terms. */
static const seq_t init_vals[4] = { (seq_t)1, (seq_t)2, (seq_t)6, (seq_t)16 };

/* Cache for computed terms. */
static seq_t *cache   = NULL;   /* pointer to the array */
static size_t cached_len = 0;   /* number of terms stored so far */
static size_t cached_cap = 0;   /* allocated capacity */

/* Ensure the cache has at least 'needed' capacity. */
static void grow_cache(size_t needed) {
    if (cached_cap >= needed) return;
    size_t new_cap = cached_cap ? (cached_cap << 1) : 4;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) abort();          /* out of memory */
    for (size_t i = 0; i < cached_len; ++i)
        new_cache[i] = cache[i];
    if (cache) free(cache);
    cache = new_cache;
    cached_cap = new_cap;
}

/* Main function implementing the sequence. */
seq_t a(idx_t n) {
    /* Return the initial terms directly. */
    if (n < 4) return init_vals[n];

    /* Initialise the cache when first needed. */
    if (cached_len == 0) {
        grow_cache(4);
        cache[0] = init_vals[0];
        cache[1] = init_vals[1];
        cache[2] = init_vals[2];
        cache[3] = init_vals[3];
        cached_len = 4;
    }

    /* Compute all missing terms up to n. */
    if (cached_len <= n) {
        /* Ensure capacity for the new terms. */
        if (cached_cap < (size_t)n + 1)
            grow_cache((size_t)n + 1);

        for (size_t i = cached_len; i <= (size_t)n; ++i) {
            /* a(i) = a(i-1) + 5*a(i-2) - a(i-3) - 2*a(i-4) */
            seq_t v = cache[i-1];
            v = v + (seq_t)5 * cache[i-2];
            v = v - cache[i-3];
            v = v - (seq_t)2 * cache[i-4];
            cache[i] = v;
        }
        cached_len = (size_t)n + 1;
    }

    return cache[n];
}

/*
 * A299265  Partial sums of A299259.
 * https://oeis.org/A299265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 6, a(2) = 19, a(3) = 45, a(4) = 90, a(5) = 159
 *            a(n) = 3*a(n-1) - 3*a(n-2) + 2*a(n-3) - 3*a(n-4) + 3*a(n-5) -
 *            a(n-6) for n > 5 (All arithmetic is performed in the seq_t
 *            type.)
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* dynamically allocated cache */
static idx_t  cache_cap = 0;     /* current capacity of the cache */
static idx_t  cache_len = 0;     /* number of values that have been computed */

/* Ensure the cache is large enough for indices up to 'needed' (inclusive). */
static void ensure_cache(idx_t needed) {
    if (needed < cache_cap)
        return;
    idx_t new_cap = cache_cap > 0 ? cache_cap : 16;
    while (new_cap <= needed)
        new_cap <<= 1;
#if defined(__cplusplus)
    seq_t *new_cache = new seq_t[new_cap];
    if (cache) {
        /* Copy existing values to the new storage. */
        for (idx_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];
        delete[] cache;
    }
#else
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    /* realloc preserves existing contents. */
#endif
    cache = new_cache;
    cache_cap = new_cap;
}

/* Compute a(n). */
seq_t a(idx_t n) {
    /* Initial values.  These will be written into the cache on the first
       call that requires storage beyond index 5. */
    static const seq_t init[6] = { 1, 6, 19, 45, 90, 159 };

    /* For the first six terms we can answer immediately. */
    if (n < 6)
        return init[n];

    /* Make sure the cache can hold at least index n. */
    ensure_cache(n);

    /* Initialise the first six values in the cache if not already done. */
    if (cache_len <= 5) {
        for (idx_t i = 0; i < 6; ++i)
            cache[i] = init[i];
        cache_len = 6;
    }

    /* Compute terms from cache_len up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t v = 3 * cache[i - 1];
        v -= 3 * cache[i - 2];
        v += 2 * cache[i - 3];
        v -= 3 * cache[i - 4];
        v += 3 * cache[i - 5];
        v -= cache[i - 6];
        cache[i] = v;
    }
    cache_len = n + 1;
    return cache[n];
}

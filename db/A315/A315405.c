/*
 * A315405  Coordination sequence Gal.3.15.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 4*a(n-2) + 3*a(n-3) - a(n-4), for n > 4 with
 *            initial values a(0) = 1, a(1) = 6, a(2) = 11, a(3) = 15, a(4) =
 *            19
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache to allow repeated calls in any order */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of elements stored (>=face) */
    static size_t cache_cap = 0;    /* current capacity of the array */

    /* initial values, stored in a small static array for speed */
    static const seq_t init[5] = { (seq_t)1, (seq_t)6, (seq_t)11, (seq_t)15, (seq_t)19 };

    /* allocate or grow cache */
    if (!cache) {
        size_t needed = n + 1;
        if (needed < 5) needed = 5;   /* we need room for the 5 init terms */
        cache_cap = needed;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure: return 0 as fallback */
        cache_size = 5;               /* we will fill first 5 entries from init */
        for (size_t i = 0; i < 5; ++i) cache[i] = init[i];
    } else if (cache_cap < (size_t)n + 1) {
        /* grow array to at least n+1, double size for future */
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;   /* allocation failure */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* if the requested index is already computed, just return */
    if ((idx_t)n < (idx_t)cache_size) {
        return cache[(size_t)n];
    }

    /* compute missing values up to n */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        seq_t t1 = cache[i-1];
        seq_t t2 = cache[i-2];
        seq_t t3 = cache[i-3];
        seq_t t4 = cache[i-4];
        /* compute 3*t1 - 4*t2 + 3*t3 - t4 avoiding underflow */
        seq_t val = (t1 * (seq_t)3) + (t3 * (seq_t)3);   /* first add the positive terms */
        val = val - (t2 * (seq_t)4);                    /* then subtract the negative terms */
        val = val - t4;
        cache[i] = val;
    }
    cache_size = (size_t)n + 1;
    return cache[(size_t)n];
}

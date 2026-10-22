/*
 * A311943  Coordination sequence Gal.4.80.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) - 2*a(n-4) + a(n-5) + a(n-7) - a(n-8)
 *            (for n ≥ 9). Initial terms: a(0)=1, a(1)=4, a(2)=8, a(3)=13,
 *            a(4)=17, a(5)=22, a(6)=27, a(7)=30, a(8)=35
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
# include <new>
#endif

/* Static cache to store already computed values; grows as needed. */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n) {
    /* Initial values for indices 0..8. */
    static const seq_t base[9] = {1,4,8,13,17,22,27,30,35};
    if (n < 9) {
        return base[n];
    }

    /* Preserve the old cache size before potentially resizing. */
    size_t oldsize = (size_t)cache_size;

    /* Ensure the cache is large enough for index n. */
    if ((idx_t)cache_size <= n) {
        size_t newsize = (size_t)n + 1;
#ifdef __cplusplus
        /* Allocate new array using C++ new. */
        seq_t* new_cache = new seq_t[newsize];
        /* Copy existing values. */
        for (size_t i = 0; i < oldsize; ++i) new_cache[i] = cache[i];
        delete[] cache;
        cache = new_cache;
#else
        /* Allocate / grow using C realloc. */
        seq_t* new_cache = (seq_t*)realloc(cache, newsize * sizeof(seq_t));
        cache = new_cache;
#endif
        /* Initialize the first 9 entries if they were not already set. */
        if (cache_size < 9) {
            for (size_t i = cache_size; i < 9; ++i) cache[i] = base[i];
        }
        cache_size = newsize;
    }

    /* Compute missing terms from the current cache. */
    for (idx_t i = (oldsize > 9 ? oldsize : 9); i <= n; ++i) {
        size_t ii = (size_t)i;
        seq_t sum = cache[ii-1] + cache[ii-3] + cache[ii-5] + cache[ii-7];
        seq_t sub = cache[ii-4] * ((seq_t)2) + cache[ii-8];
        cache[ii] = sum - sub;
    }

    return cache[(size_t)n];
}

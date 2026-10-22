/*
 * A392349  a(n) = Sum_{k=0..floor(3*n/8)} binomial(3*n-7*k,k).
 * https://oeis.org/A392349
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 36, u64 n <= 72, u128 n <= 143,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A392349
   Formula: a(n) = a(n-1) + 3*a(n-3) + a(n-8)
   Initial values: a(0)=1, a(1)=1, a(2)=1, a(3)=3, a(4)=6,
                     a(5)=9, a(6)=18, a(7)=36 */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* Local base values for n < 8 */
    static const seq_t base[8] = {1, 1, 1, 3, 6, 9, 18, 36};

    /* Return base values directly */
    if (n < 8) return base[n];

    /* Static cache for previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* allocated capacity */
    static idx_t cache_size = 0;  /* next empty index */

    /* Initial allocation and seed with base values */
    if (!cache) {
        cache_cap = 16;  /* start with small capacity, will grow if needed */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;         /* out of memory: return 0 (unlikely) */
        for (idx_t i = 0; i < 8; ++i) cache[i] = base[i];
        cache_size = 8;
    }

    /* Grow the cache if we need more space */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (n >= new_cap) new_cap <<= 1; /* double until enough */
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return 0;          /* realloc failed */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing values up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t val = cache[i-1] + ((seq_t)3 * cache[i-3]) + cache[i-8];
        cache[i] = val;
    }
    cache_size = n + 1;

    return cache[n];
}

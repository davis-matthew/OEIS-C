/*
 * A269202  Number of 2 X n 0..3 arrays with some element plus some
 *          horizontally, diagonally or antidiagonally adjacent neighbor
 *          totalling three no more than once.
 * https://oeis.org/A269202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A269202
   Formula: a(n) = 10*a(n-1) - 13*a(n-2) - 60*a(n-3) - 36*a(n-4),
   with a(1)=16, a(2)=180, a(3)=1284, a(4)=9612. */

#include <stdlib.h>
#include <stdbool.h>

/* Return the nth term of A269202, where a(1)=16 is the first term. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;    /* 1‑based array of already computed terms */
    static idx_t cache_size = 0;   /* size of cache (number of allocated slots) */

    if (n == 0)   /* undefined for the OEIS offset, return zero for safety */
        return (seq_t)0;

    /* Grow the cache if needed */
    if (n >= cache_size) {
        idx_t new_size = n + 1;   /* Allocate slots 0..n (slot 0 unused) */

#if defined(__cplusplus)
        /* In C++ use new[]/delete[] to construct objects correctly. */
        seq_t *new_cache = new seq_t[new_size];
        for (idx_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];
        delete[] cache;
        cache = new_cache;
#else
        /* In C use malloc/realloc which works for plain integral types. */
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        cache = new_cache;
#endif

        /* Compute the terms that are now missing */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 1)            cache[i] = (seq_t)16;
            else if (i == 2)       cache[i] = (seq_t)180;
            else if (i == 3)       cache[i] = (seq_t)1284;
            else if (i == 4)       cache[i] = (seq_t)9612;
            else                   cache[i] = (seq_t)10 * cache[i-1]
                                  - (seq_t)13 * cache[i-2]
                                  - (seq_t)60 * cache[i-3]
                                  - (seq_t)36 * cache[i-4];
        }

        cache_size = new_size;
    }

    return cache[n];
}

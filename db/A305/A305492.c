/*
 * A305492  a(n) = ((1 + y)^n - (1 - y)^n)/y with y = sqrt(8).
 * https://oeis.org/A305492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 7*a(n-2), a(0) = 0, a(1) = 2
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 953 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

/* Include utility routines for dynamic arrays */
#include <stdlib.h>

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t  cache_cap = 0;   /* allocated capacity in seq_t elements */
    static idx_t  cache_len = 0;   /* number of elements currently stored */

    /* initialise cache if needed */
    if (cache_len == 0) {
        cache_cap = 2;                      /* enough for a(0) and a(1) */
        cache = seq_alloc(cache_cap);
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)2;
        cache_len = 2;
    }

    /* return if already computed */
    if (n < cache_len)
        return cache[n];

    /* grow cache if necessary */
    if (n + 1 > cache_cap) {
        cache = seq_realloc(cache, cache_cap, (size_t)(n + 1));
        cache_cap = n + 1;
    }

    /* compute missing values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = (seq_t)2 * cache[i - 1] + (seq_t)7 * cache[i - 2];
    }
    cache_len = n + 1;

    return cache[n];
}

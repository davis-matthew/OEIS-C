/*
 * A318236  a(n) = (3*2^(4*n+3) + 1)/5.
 * https://oeis.org/A318236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*2^(4*n+3) + 1)/5
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 198 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=5, a(1)=77; a(n)=17*a(n-1)-16*a(n-2) */

#include <stddef.h>              /* size_t */
#include <stdint.h>              /* uint64_t */
#include <stdlib.h>

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
    /* Static cache of computed terms */
    static seq_t *cache   = NULL;   /* array holding terms a(0), a(1), … */
    static size_t cache_size = 0;   /* allocated size of cache (in elements) */
    static idx_t known = 0;         /* largest index for which a(i) is stored */

    /* Ensure we have the initial two terms available */
    if (cache_size < 2) {
        cache = seq_realloc(cache, cache_size, 2);
        /* Provided that seq_realloc correctly constructs seq_t objects */
        cache[0] = (seq_t)5;      /* a(0) */
        cache[1] = (seq_t)77;     /* a(1) */
        cache_size = 2;
        known = 1;
    }

    /* Grow the cache if necessary */
    if ((size_t)(n + 1) > cache_size) {
        cache = seq_realloc(cache, cache_size, (size_t)(n + 1));
        cache_size = (size_t)(n + 1);
    }

    /* Compute any missing terms up to n using the recurrence */
    if (n > known) {
        for (idx_t i = known + 1; i <= n; ++i) {
            /* a(i) = 17*a(i-1) - 16*a(i-2) */
            cache[i] = (seq_t)17 * cache[i - 1] - (seq_t)16 * cache[i - 2];
        }
        known = n;
    }

    return cache[n];
}

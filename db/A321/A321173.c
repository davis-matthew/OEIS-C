/*
 * A321173  a(n) = -2*a(n-1) + a(n-2) + a(n-3), a(0) = -1, a(1) = 3, a(2) =
 *          -9.
 * https://oeis.org/A321173
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -2*a(n-1) + a(n-2) + a(n-3); Initial values: a(0) = -1;
 *            a(1) = 3; a(2) = -9
 * Range:     i8 n <= 5, i16 n <= 12, i32 n <= 25, i64 n <= 53, i128 n <= 108,
 *            big any n
 * Validated: standard, pass, 47 terms through n = 954 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                /* for size_t */
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
    /* static cache for memoisation (thread‑unsafe, but allowed) */
    static seq_t *cache = NULL;    /* pointer to array of seq_t */
    static size_t cache_cap = 0;   /* allocated capacity */
    static idx_t last_computed = (idx_t)-1; /* index of highest computed term */

    /* initialisation */
    if (cache == NULL) {
        cache_cap = 4;                            /* at least 4 slots */
        cache = seq_alloc(cache_cap);
        cache[0] = -1;
        cache[1] = 3;
        cache[2] = -9;
        last_computed = 2;
    }

    /* special case for requested index within the cached range */
    if (n <= last_computed) {
        return cache[n];
    }

    /* ensure capacity for n+1 elements */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)(n + 1) > new_cap)
            new_cap *= 2;
        cache = seq_realloc(cache, cache_cap, new_cap);
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t val = -2 * cache[i - 1] + cache[i - 2] + cache[i - 3];
        cache[i] = val;
    }
    last_computed = n;

    return cache[n];
}

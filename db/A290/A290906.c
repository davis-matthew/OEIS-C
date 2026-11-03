/*
 * A290906  p-INVERT of the positive integers, where p(S) = 1 - 3*S^2.
 * https://oeis.org/A290906
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 3*a(n-2) + 4*a(n-3) - a(n-4), a(0)=0, a(1)=3,
 *            a(2)=12, a(3)=39
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 26 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
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

static seq_t *cache = NULL;   /* memoized values */
static idx_t cache_len = 0;   /* number of values computed */
static idx_t cache_cap = 0;   /* capacity of cache array */

/* Ensure cache has entries up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n < cache_len) return;

    /* First allocation */
    if (cache == NULL) {
        size_t cap = 8;
        if ((size_t)(n + 1) > cap) cap = (size_t)(n + 1);
        cache = seq_alloc(cap);               /* allocate array */
        cache_cap = (idx_t)cap;
        cache_len = 0;
    }

    /* Grow if necessary */
    if ((size_t)(n + 1) > (size_t)cache_cap) {
        size_t new_cap = cache_cap ? (size_t)cache_cap : 8;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        cache = seq_realloc(cache, cache_cap, new_cap);
        cache_cap = (idx_t)new_cap;
    }

    /* Fill missing elements */
    for (idx_t i = cache_len; i <= n; ++i) {
        switch (i) {
            case 0: cache[i] = 0; break;
            case 1: cache[i] = 3; break;
            case 2: cache[i] = 12; break;
            case 3: cache[i] = 39; break;
            default:
                cache[i] = 4 * cache[i-1] - 3 * cache[i-2]
                         + 4 * cache[i-3] - cache[i-4];
                break;
        }
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}

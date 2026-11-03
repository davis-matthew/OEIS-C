/*
 * A245384  a(n) = (1 + a(n-1)) * a(n-2) * a(n-3) with a(1) = 1, a(2) = 2,
 *          a(3) = 3.
 * https://oeis.org/A245384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1 + a(n-1)) * a(n-2) * a(n-3) initial values: a(1)=1,
 *            a(2)=2, a(3)=3 (computed as a recurrence; memoised for arbitrary
 *            access order)
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 8, u128 n <= 10,
 *            big any n
 * Validated: partial, pass, 11 terms through n = 11 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>          /* size_t */
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
    /* static cache: indices are 1‑based; cache[0] unused          */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated capacity */
    static size_t len   = 0;   /* largest n with cache[n] valid */

    /* initialise cache with first three terms */
    if (len == 0) {
        cap   = 4;                      /* indices 0..3 */
        cache = seq_alloc(cap);
        if (!cache) abort();            /* construction failure */

        cache[0] = 0;   /* unused (no a(0) in OEIS definition) */
        cache[1] = 1;
        cache[2] = 2;
        cache[3] = 3;
        len = 3;
    }

    /* return cached value if already computed */
    if ((size_t)n <= len)
        return cache[n];

    /* grow cache to at least n+1 elements (indices 0..n) */
    if ((size_t)n >= cap) {
        size_t newcap = cap * 2;
        while (newcap <= (size_t)n) newcap <<= 1;
        seq_t *tmp = seq_realloc(cache, cap, newcap);
        if (!tmp) abort();            /* reallocation failure */
        cache = tmp;
        cap   = newcap;
    }

    /* compute missing terms iteratively */
    for (idx_t i = (idx_t)(len + 1); i <= n; ++i) {
        seq_t val = (1 + cache[(size_t)i - 1]) * cache[(size_t)i - 2] * cache[(size_t)i - 3];
        cache[(size_t)i] = val;
    }
    len = n;
    return cache[n];
}

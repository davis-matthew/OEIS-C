/*
 * A281185  a(0)=0, a(1)=1, a(2)=0; thereafter, a(2n) = a(n) + a(n+1) for n >=
 *          2, a(2n+1) = a(n) for n >= 1.
 * https://oeis.org/A281185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, easy
 * Formula:   a(0)=0, a(1)=1, a(2)=0; for n>=2: a(2n) = a(n) + a(n+1); for
 *            n>=1: a(2n+1) = a(n)
 * Range:     u8 n <= 12287, u16 n <= 25000, u32 n <= 25000, u64 n <= 25000,
 *            u128 n <= 25000, big any n
 * Validated: standard, pass, 116 terms through n = 9787 (2026-11-01)
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

seq_t a(idx_t n)
{
    /* Static cache of already computed values */
    static seq_t *cache = NULL;
    static idx_t max_computed = 0;   /* largest n whose value is cached */

    /* If nothing cached yet, allocate and fill the base cases */
    if (cache == NULL) {
        cache = seq_alloc(n + 1);
        if (n >= 0) cache[0] = 0;
        if (n >= 1) cache[1] = 1;
        if (n >= 2) cache[2] = 0;
        max_computed = (n >= 2) ? 2 : n;
    }

    /* If a larger index is requested, grow the cache and compute new values */
    if (n > max_computed) {
        cache = seq_realloc(cache, max_computed + 1, n + 1);
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            if (i == 0) {
                /* already set in initial allocation */
            } else if (i == 1) {
                cache[1] = 1;
            } else if (i == 2) {
                cache[2] = 0;
            } else if ((i & 1) == 0) {        /* i is even */
                idx_t k = i / 2;
                cache[i] = cache[k] + cache[k + 1];
            } else {                          /* i is odd */
                idx_t k = (i - 1) / 2;
                cache[i] = cache[k];
            }
        }
        max_computed = n;
    }

    return cache[n];
}

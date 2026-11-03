/*
 * A276160  A recurrence of order 3 : a(0)=a(1)=a(2)=1 ; a(n) = (a(n-1)^2 +
 *          a(n-2)^2 + a(n-1) + a(n-2) + 1)/a(n-3).
 * https://oeis.org/A276160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Validated: standard, pass, 17 terms through n = 16 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A276160: a(0)=a(1)=a(2)=1; a(n)=7*a(n-1)*a(n-2)-a(n-3)-1 for n≥3 */
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

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_last = 2;

/* Return the nth term of A276160. */
seq_t a(idx_t n)
{
    /* Base cases. */
    if (n == 0 || n == 1 || n == 2)
        return (seq_t)1;

    /* If we have a cache and n is already computed, return it. */
    if (cache && n <= cache_last)
        return cache[n];

    /* Ensure the cache is large enough. */
    idx_t needed = n + 1;                     /* number of elements required */
    if (!cache) {
        cache = seq_alloc(needed);
        cache_cap = (size_t)needed;
        cache[0] = cache[1] = cache[2] = (seq_t)1;
        cache_last = 2;
    } else if ((size_t)needed > cache_cap) {
        cache = seq_realloc(cache, cache_cap, (size_t)needed);
        cache_cap = (size_t)needed;
    }

    /* Compute terms up to n, if needed. */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        seq_t a_nm1 = cache[i - 1];
        seq_t a_nm2 = cache[i - 2];
        seq_t a_nm3 = cache[i - 3];

        seq_t val = (seq_t)7;
        val = val * a_nm1;
        val = val * a_nm2;
        val = val - a_nm3;
        val = val - (seq_t)1;

        cache[i] = val;
    }
    cache_last = n;

    return cache[n];
}

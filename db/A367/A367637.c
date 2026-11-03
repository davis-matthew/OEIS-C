/*
 * A367637  G.f. A(x) satisfies A(x) = 1 / (1 - x * A(x^6)).
 * https://oeis.org/A367637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor((n-1)/6)} a(k) * a(n-1-6*k), a(0) = 1
 * Range:     u8 n <= 26, u16 n <= 48, u32 n <= 92, u64 n <= 180,
 *            u128 n <= 357, big any n
 * Validated: standard, pass, 68 terms through n = 4599 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
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

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t *cache = NULL;      /* cache array of computed values      */
static idx_t cached_cap = 0;     /* current capacity of 'cache'        */
static idx_t cached_len = 0;     /* number of values already filled    */

/* Ensure the cache can hold values up to index 'n' (inclusive).  */
static void
ensure_cache(idx_t n)
{
    if (cached_cap <= n) {
        /* Grow capacity, doubling until it exceeds 'n'. */
        idx_t new_cap = (cached_cap == 0) ? 1 : cached_cap * 2;
        while (new_cap <= n) new_cap *= 2;
        /* Reallocate using the helper that knows how to handle
           arbitrary-precision seq_t types. */
        cache = seq_realloc(cache, (size_t)cached_cap, (size_t)new_cap);
        cached_cap = new_cap;
    }
}

seq_t
a(idx_t n)
{
    /* If we already have computed up to 'n', just return it. */
    if (n < cached_len)
        return cache[n];

    /* Make sure the cache can hold all indices up to 'n'. */
    ensure_cache(n);

    /* Compute all missing values sequentially. */
    for (idx_t i = cached_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;          /* a(0) = 1 */
        } else {
            seq_t sum = 0;
            idx_t maxk = (i - 1) / 6;     /* floor((i-1)/6) */
            for (idx_t k = 0; k <= maxk; ++k) {
                idx_t j = i - 1 - 6 * k;
                sum += cache[k] * cache[j];
            }
            cache[i] = sum;
        }
    }
    cached_len = n + 1;
    return cache[n];
}

/*
 * A397779  2-adic valuation of the triple factorial of n.
 * https://oeis.org/A397779
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-3) + v2(n) for n ≥ 3 a(0) = 0, a(1) = 0, a(2) = 1
 *            (v2(k) = exponent of 2 in k)
 * Range:     u8 n <= 763, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Helper: exponent of 2 dividing n. */
static inline uint64_t v2_uint64(idx_t n) {
    return (n == 0) ? 0ULL : __builtin_ctzll((unsigned long long)n);
}

/* Static cache for memoisation. */
static seq_t *a_cache      = NULL;   /* storage for computed terms */
static idx_t  a_cache_len   = 0;     /* number of valid entries in the cache */
static idx_t  a_cache_cap   = 0;     /* allocated capacity */

/* Grow the cache to hold at least 'new_cap' entries. */
static void a_cache_grow(idx_t new_cap) {
    if (new_cap <= a_cache_cap) return;

    seq_t *new_ptr;
#if defined(__cplusplus)
    new_ptr = new seq_t[new_cap];
#else
    new_ptr = (seq_t*)calloc(new_cap, sizeof(seq_t));
#endif
    if (!new_ptr) return; /* allocation failure – keep old cache */

    /* Copy existing elements to the new array. */
    for (idx_t i = 0; i < a_cache_len; ++i) {
        new_ptr[i] = a_cache[i];
    }

    /* Free previous storage. */
    if (a_cache) {
#if defined(__cplusplus)
        delete[] a_cache;
#else
        free(a_cache);
#endif
    }

    a_cache = new_ptr;
    a_cache_cap = new_cap;
}

seq_t a(idx_t n) {
    /* Ensure we can store at least the first three base values. */
    if (a_cache_cap < 3) a_cache_grow(3);

    /* Return a cached value if already computed. */
    if (n < a_cache_len) return a_cache[n];

    /* Extend cache size if necessary. */
    if (n + 1 > a_cache_cap) a_cache_grow(n + 1);

    /* Compute any missing terms up to n. */
    for (idx_t i = a_cache_len; i <= n; ++i) {
        if (i < 3) {                      /* base cases */
            if (i == 0) a_cache[0] = 0;
            else if (i == 1) a_cache[1] = 0;
            else             a_cache[2] = 1;
        } else {                          /* recurrence */
            a_cache[i] = a_cache[i-3] + (seq_t)v2_uint64(i);
        }
    }

    a_cache_len = n + 1;
    return a_cache[n];
}

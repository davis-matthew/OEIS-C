/*
 * A260710  Expansion of 1/(1 - x - x^2 - x^4 + x^5 + x^7).
 * https://oeis.org/A260710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-4) - a(n-5) - a(n-7) Initial
 *            values: a(0)=1, a(1)=1, a(2)=2, a(3)=3, a(4)=6, a(5)=9, a(6)=16
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

#ifdef __cplusplus
/* C++ dynamic allocation for arbitrary‑precision seq_t */
static inline seq_t* allocate(size_t n) { return new seq_t[n]; }
static inline void freep(seq_t* p)          { delete[] p; }
#else
/* C allocation */
static inline seq_t* allocate(size_t n) { return calloc(n, sizeof(seq_t)); }
static inline void freep(seq_t* p)          { free(p); }
#endif

/* Compute a single term from the recurrence using values already in cache */
static inline seq_t compute_term(idx_t i, seq_t *c) {
    seq_t tmp = c[i-1];
    tmp += c[i-2];
    tmp += c[i-4];
    tmp -= c[i-5];
    tmp -= c[i-7];
    return tmp;
}

/* Ensure that cache has at least n+1 entries, extending it if necessary */
static void ensure_cache(idx_t n) {
    if (cache && n < cache_size) return;

    /* Initialize the first seven values if cache is empty */
    if (!cache) {
        cache_size = 7;
        cache = allocate(cache_size);
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 2;
        cache[3] = 3;
        cache[4] = 6;
        cache[5] = 9;
        cache[6] = 16;
    }

    /* Extend cache to hold terms up to n */
    if (n + 1 <= cache_size) return;
    size_t new_size = (size_t)(n + 1);
    seq_t *new_cache = allocate(new_size);
    /* Copy old values */
    for (idx_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];
    freep(cache);
    cache = new_cache;

    /* Compute new terms */
    for (idx_t i = cache_size; i <= n; ++i) {
        cache[i] = compute_term(i, cache);
    }
    cache_size = new_size;
}

/* Return a(n) for any n >= 0, memoised across calls */
seq_t a(idx_t n) {
    ensure_cache(n);
    return cache[n];
}

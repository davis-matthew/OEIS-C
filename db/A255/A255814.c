/*
 * A255814  Numbers of words on {0,1,2,3,4,} having no isolated zeros.
 * https://oeis.org/A255814
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A255814: Numbers of words on {0,1,2,3,4} having no isolated zeros
   Recurrence:  a(n+3) = 5*a(n+2) - 4*a(n+1) + 4*a(n)
   Initial values:  a(0)=1, a(1)=4, a(2)=17 */
#include <stdlib.h>

static seq_t *cache = NULL;          /* memoisation array */
static idx_t  cache_size = 0;        /* allocated capacity */
static idx_t  cached_max = 0;        /* maximum index with a value */

/* Ensure that cache has room for indices up to n */
static void ensure_capacity(idx_t n) {
    if (n < cache_size) return;

    size_t new_cap = cache_size ? cache_size : 8;
    while (new_cap <= (size_t)n) new_cap <<= 1;

    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
    if (!new_cache) exit(EXIT_FAILURE);   /* OOM guard */
    cache = new_cache;
    cache_size = (idx_t)new_cap;
}

/* Compute a(n) using memoisation and the linear recurrence */
seq_t a(idx_t n) {
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)17;

    /* Initialise cache on first call */
    if (!cache) {
        ensure_capacity(8);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)17;
        cached_max = 2;
    }

    /* Expand cache up to n if necessary */
    ensure_capacity(n);

    /* Fill missing terms up to n */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        /* recurrence: a(i) = 5*a(i-1) - 4*a(i-2) + 4*a(i-3) */
        seq_t term = cache[i-1] * (seq_t)5 + cache[i-3] * (seq_t)4;
        cache[i] = term - cache[i-2] * (seq_t)4;
    }
    cached_max = n;

    return cache[n];
}

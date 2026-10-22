/*
 * A306777  a(0)=0, a(1)=1, a(n) = a(n-1) + sum(a(floor(n/d)), d=2^x,
 *          x=1...n).
 * https://oeis.org/A306777
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + sum_{d=2^k ≤ n} a(floor(n/d)) with a(0)=0,
 *            a(1)=1 Sequence A306777
 * Range:     u8 n <= 18, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cache of computed a(n) */
static idx_t cache_size = 0;     /* number of elements initialized in cache */
static size_t cache_cap = 0;     /* allocated capacity of cache */

/* Allocate or grow the cache to accommodate at least n+1 entries */
static void ensure_cache(idx_t n)
{
    /* Expand capacity by powers of two until enough */
    size_t needed = (size_t)n + 1;
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Public function required by the build */
seq_t a(idx_t n)
{
    if (n < cache_size) return cache[n];
    ensure_cache(n);

    /* Initialize first two terms if needed */
    if (cache_size == 0) {
        cache[0] = 0;
        cache_size = 1;
    }
    if (cache_size <= 1 && n >= 1) {
        cache[1] = 1;
        cache_size = 2;
    }

    /* Compute missing values up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t val = cache[i - 1];          /* a(i-1) */
        idx_t m = i >> 1;                  /* floor(i/2) */
        while (m > 0) {                    /* iterate over powers of 2 */
            val += cache[m];
            m >>= 1;                       /* next floor(i/4), i/8, ... */
        }
        cache[i] = val;
    }
    cache_size = n + 1;
    return cache[n];
}

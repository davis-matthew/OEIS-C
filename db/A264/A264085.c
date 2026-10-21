/*
 * A264085  Number of (n+1) X (2+1) arrays of permutations of 0..n*3+2 with
 *          each element having index change +-(.,.) 0,0 0,1 or 2,-2.
 * https://oeis.org/A264085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 4*a(n-3) + a(n-4) (n>=5) Initial values:
 *            a(1)=9, a(2)=39, a(3)=169, a(4)=715 Off‑set: 1 Implementation
 *            uses linear memoisation to answer any call order in O(n) total
 *            time
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* memoised sequence values */
static idx_t cache_capacity = 0; /* current allocated size (>= needed+1) */

/* Ensure the cache can hold values up to index 'needed' (inclusive) */
static void
ensure_cache(idx_t needed)
{
    if (needed < cache_capacity) return;
    idx_t new_cap = cache_capacity > 0 ? cache_capacity : 64;
    while (new_cap <= needed) new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
    if (!new_cache) abort();               /* allocation failed */
    /* Zero newly allocated entries: works for any seq_t type */
    for (idx_t i = cache_capacity; i < new_cap; ++i) new_cache[i] = 0;
    cache       = new_cache;
    cache_capacity = new_cap;
}

/* Public function required by the test harness */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                   /* undefined for offset 1, but safe */
    ensure_cache(n + 1);

    /* Initialize the first four terms once */
    if (cache[1] == 0) {
        cache[1] = (seq_t)9;
        cache[2] = (seq_t)39;
        cache[3] = (seq_t)169;
        cache[4] = (seq_t)715;
    }

    /* Compute missing terms up to n */
    if (cache[n] == 0) {
        for (idx_t i = 5; i <= n; ++i) {
            cache[i] = 4 * cache[i - 1] + 4 * cache[i - 3] + cache[i - 4];
        }
    }

    return cache[n];
}
